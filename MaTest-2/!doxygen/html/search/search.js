ould not allocate skb\n",
				  ring->dev->name);
			if (first_rxdp) {
				wmb();
				first_rxdp->Control_1 |= RXD_OWN_XENA;
			}
			swstats->mem_alloc_fail_cnt++;

			return -ENOMEM ;
		}
		swstats->mem_allocated += skb->truesize;

		if (ring->rxd_mode == RXD_MODE_1) {
			/* 1 buffer mode - normal operation mode */
			rxdp1 = (struct RxD1 *)rxdp;
			memset(rxdp, 0, sizeof(struct RxD1));
			skb_reserve(skb, NET_IP_ALIGN);
			rxdp1->Buffer0_ptr =
				pci_map_single(ring->pdev, skb->data,
					       size - NET_IP_ALIGN,
					       PCI_DMA_FROMDEVICE);
			if (pci_dma_mapping_error(nic->pdev,
						  rxdp1->Buffer0_ptr))
				goto pci_map_failed;

			rxdp->Control_2 =
				SET_BUFFER0_SIZE_1(size - NET_IP_ALIGN);
			rxdp->Host_Control = (unsigned long)skb;
		} else if (ring->rxd_mode == RXD_MODE_3B) {
			/*
			 * 2 buffer mode -
			 * 2 buffer mode provides 128
			 * byte aligned receive buffers.
			 */

			rxdp3 = (struct RxD3 *)rxdp;
			/* save buffer pointers to avoid frequent dma mapping */
			Buffer0_ptr = rxdp3->Buffer0_ptr;
			Buffer1_ptr = rxdp3->Buffer1_ptr;
			memset(rxdp, 0, sizeof(struct RxD3));
			/* restore the buffer pointers for dma sync*/
			rxdp3->Buffer0_ptr = Buffer0_ptr;
			rxdp3->Buffer1_ptr = Buffer1_ptr;

			ba = &ring->ba[block_no][off];
			skb_reserve(skb, BUF0_LEN);
			tmp = (u64)(unsigned long)skb->data;
			tmp += ALIGN_SIZE;
			tmp &= ~ALIGN_SIZE;
			skb->data = (void *) (unsigned long)tmp;
			skb_reset_tail_pointer(skb);

			if (from_card_up) {
				rxdp3->Buffer0_ptr =
					pci_map_single(ring->pdev, ba->ba_0,
						       BUF0_LEN,
						       PCI_DMA_FROMDEVICE);
				if (pci_dma_mapping_error(nic->pdev,
							  rxdp3->Buffer0_ptr))
					goto pci_map_failed;
			} else
				pci_dma_sync_single_for_device(ring->pdev,
							       (dma_addr_t)rxdp3->Buffer0_ptr,
							       BUF0_LEN,
							       PCI_DMA_FROMDEVICE);

			rxdp->Control_2 = SET_BUFFER0_SIZE_3(BUF0_LEN);
			if (ring->rxd_mode == RXD_MODE_3B) {
				/* Two buffer mode */

				/*
				 * Buffer2 will have L3/L4 header plus
				 * L4 payload
				 */
				rxdp3->Buffer2_ptr = pci_map_single(ring->pdev,
								    skb->data,
								    ring->mtu + 4,
								    PCI_DMA_FROMDEVICE);

				if (pci_dma_mapping_error(nic->pdev,
							  rxdp3->Buffer2_ptr))
					goto pci_map_failed;

				if (from_card_up) {
					rxdp3->Buffer1_ptr =
						pci_map_single(ring->pdev,
							       ba->ba_1,
							       BUF1_LEN,
							       PCI_DMA_FROMDEVICE);

					if (pci_dma_mapping_error(nic->pdev,
								  rxdp3->Buffer1_ptr)) {
						pci_unmap_single(ring->pdev,
								 (dma_addr_t)(unsigned long)
								 skb->data,
								 ring->mtu + 4,
								 PCI_DMA_FROMDEVICE);
						goto pci_map_failed;
					}
				}
				rxdp->Control_2 |= SET_BUFFER1_SIZE_3(1);
				rxdp->Control_2 |= SET_BUFFER2_SIZE_3
					(ring->mtu + 4);
			}
			rxdp->Control_2 |= s2BIT(0);
			rxdp->Host_Control = (unsigned long) (skb);
		}
		if (alloc_tab & ((1 << rxsync_frequency) - 1))
			rxdp->Control_1 |= RXD_OWN_XENA;
		off++;
		if (off == (ring->rxd_count + 1))
			off = 0;
		ring->rx_curr_put_info.offset = off;

		rxdp->Control_2 |= SET_RXD_MARKER;
		if (!(alloc_tab & ((1 << rxsync_frequency) - 1))) {
			if (first_rxdp) {
				wmb();
				first_rxdp->Control_1 |= RXD_OWN_XENA;
			}
			first_rxdp = rxdp;
		}
		ring->rx_bufs_left += 1;
		alloc_tab++;
	}

end:
	/* Transfer ownership of first descriptor to adapter just before
	 * exiting. Before that, use memory barrier so that ownership
	 * and other fields are seen by adapter correctly.
	 */
	if (first_rxdp) {
		wmb();
		first_rxdp->Control_1 |= RXD_OWN_XENA;
	}

	return SUCCESS;

pci_map_failed:
	swstats->pci_map_fail_cnt++;
	swstats->mem_freed += skb->truesize;
	dev_kfree_skb_irq(skb);
	return -ENOMEM;
}

static void free_rxd_blk(struct s2io_nic *sp, int ring_no, int blk)
{
	struct net_device *dev = sp->dev;
	int j;
	struct sk_buff *skb;
	struct RxD_t *rxdp;
	struct buffAdd *ba;
	struct RxD1 *rxdp1;
	struct RxD3 *rxdp3;
	struct mac_info *mac_control = &sp->mac_control;
	struct stat_block *stats = mac_control->stats_info;
	struct swStat *swstats = &stats->sw_stat;

	for (j = 0 ; j < rxd_count[sp->rxd_mode]; j++) {
		rxdp = mac_control->rings[ring_no].
			rx_blocks[blk].rxds[j].virt_addr;
		skb = (struct sk_buff *)((unsigned long)rxdp->Host_Control);
		if (!skb)
			continue;
		if (sp->rxd_mode == RXD_MODE_1) {
			rxdp1 = (struct RxD1 *)rxdp;
			pci_unmap_single(sp->pdev,
					 (dma_addr_t)rxdp1->Buffer0_ptr,
					 dev->mtu +
					 HEADER_ETHERNET_II_802_3_SIZE +
					 HEADER_802_2_SIZE + HEADER_SNAP_SIZE,
					 PCI_DMA_FROMDEVICE);
			memset(rxdp, 0, sizeof(struct RxD1));
		} else if (sp->rxd_mode == RXD_MODE_3B) {
			rxdp3 = (struct RxD3 *)rxdp;
			ba = &mac_control->rings[ring_no].ba[blk][j];
			pci_unmap_single(sp->pdev,
					 (dma_addr_t)rxdp3->Buffer0_ptr,
					 BUF0_LEN,
					 PCI_DMA_FROMDEVICE);
			pci_unmap_single(sp->pdev,
					 (dma_addr_t)rxdp3->Buffer1_ptr,
					 BUF1_LEN,
					 PCI_DMA_FROMDEVICE);
			pci_unmap_single(sp->pdev,
					 (dma_addr_t)rxdp3->Buffer2_ptr,
					 dev->mtu + 4,
					 PCI_DMA_FROMDEVICE);
			memset(rxdp, 0, sizeof(struct RxD3));
		}
		swstats->mem_freed += skb->truesize;
		dev_kfree_skb(skb);
		mac_control->rings[ring_no].rx_bufs_left -= 1;
	}
}

/**
 *  free_rx_buffers - Frees all Rx buffers
 *  @sp: device private variable.
 *  Description:
 *  This function will free all Rx buffers allocated by host.
 *  Return Value:
 *  NONE.
 */

static void free_rx_buffers(struct s2io_nic *sp)
{
	struct net_device *dev = sp->dev;
	int i, blk = 0, buf_cnt = 0;
	struct config_param *config = &sp->config;
	struct mac_info *mac_control = &sp->mac_control;

	for (i = 0; i < config->rx_ring_num; i++) {
		struct ring_info *ring = &mac_control->rings[i];

		for (blk = 0; blk < rx_ring_sz[i]; blk++)
			free_rxd_blk(sp, i, blk);

		ring->rx_curr_put_info.block_index = 0;
		ring->rx_curr_get_info.block_index = 0;
		ring->rx_curr_put_info.offset = 0;
		ring->rx_curr_get_info.offset = 0;
		ring->rx_bufs_left = 0;
		DBG_PRINT(INIT_DBG, "%s: Freed 0x%x Rx Buffers on ring%d\n",
			  dev->name, buf_cnt, i);
	}
}

static int s2io_chk_rx_buffers(struct s2io_nic *nic, struct ring_info *ring)
{
	if (fill_rx_buffers(nic, ring, 0) == -ENOMEM) {
		DBG_PRINT(INFO_DBG, "%s: Out of memory in Rx Intr!!\n",
			  ring->dev->name);
	}
	return 0;
}

/**
 * s2io_poll - Rx interrupt handler for NAPI support
 * @napi : pointer to the napi structure.
 * @budget : The number of packets that were budgeted to be processed
 * during  one pass through the 'Poll" function.
 * Description:
 * Comes into picture only if NAPI support has been incorporated. It does
 * the same thing that rx_intr_handler does, but not in a interrupt context
 * also It will process only a given number of packets.
 * Return value:
 * 0 on success and 1 if there are No Rx packets to be processed.
 */

static int s2io_poll_msix(struct napi_struct *napi, int budget)
{
	struct ring_info *ring = container_of(napi, struct ring_info, napi);
	struct net_device *dev = ring->dev;
	int pkts_processed = 0;
	u8 __iomem *addr = NULL;
	u8 val8 = 0;
	struct s2io_nic *nic = netdev_priv(dev);
	struct XENA_dev_config __iomem *bar0 = nic->bar0;
	int budget_org = budget;

	if (unlikely(!is_s2io_card_up(nic)))
		return 0;

	pkts_processed = rx_intr_handler(ring, budget);
	s2io_chk_rx_buffers(nic, ring);

	if (pkts_processed < budget_org) {
		napi_complete(napi);
		/*Re Enable MSI-Rx Vector*/
		addr = (u8 __iomem *)&bar0->xmsi_mask_reg;
		addr += 7 - ring->ring_no;
		val8 = (ring->ring_no == 0) ? 0x3f : 0xbf;
		writeb(val8, addr);
		val8 = readb(addr);
	}
	return pkts_processed;
}

static int s2io_poll_inta(struct napi_struct *napi, int budget)
{
	struct s2io_nic *nic = container_of(napi, struct s2io_nic, napi);
	int pkts_processed = 0;
	int ring_pkts_processed, i;
	struct XENA_dev_config __iomem *bar0 = nic->bar0;
	int budget_org = budget;
	struct config_param *config = &nic->config;
	struct mac_info *mac_control = &nic->mac_control;

	if (unlikely(!is_s2io_card_up(nic)))
		return 0;

	for (i = 0; i < config->rx_ring_num; i++) {
		struct ring_info *ring = &mac_control->rings[i];
		ring_pkts_processed = rx_intr_handler(ring, budget);
		s2io_chk_rx_buffers(nic, ring);
		pkts_processed += ring_pkts_processed;
		budget -= ring_pkts_processed;
		if (budget <= 0)
			break;
	}
	if (pkts_processed < budget_org) {
		napi_complete(napi);
		/* Re enable the Rx interrupts for the ring */
		writeq(0, &bar0->rx_traffic_mask);
		readl(&bar0->rx_traffic_mask);
	}
	return pkts_processed;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
/**
 * s2io_netpoll - netpoll event handler entry point
 * @dev : pointer to the device structure.
 * Description:
 * 	This function will be called by upper layer to check for events on the
 * interface in situations where interrupts are disabled. It is used for
 * specific in-kernel networking tasks, such as remote consoles and kernel
 * debugging over the network (example netdump in RedHat).
 */
static void s2io_netpoll(struct net_device *dev)
{
	struct s2io_nic *nic = netdev_priv(dev);
	struct XENA_dev_config __iomem *bar0 = nic->bar0;
	u64 val64 = 0xFFFFFFFFFFFFFFFFULL;
	int i;
	struct config_param *config = &nic->config;
	struct mac_info *mac_control = &nic->mac_control;

	if (pci_channel_offline(nic->pdev))
		return;

	disable_irq(dev->irq);

	writeq(val64, &bar0->rx_traffic_int);
	writeq(val64, &bar0->tx_traffic_int);

	/* we need to free up the transmitted skbufs or else netpoll will
	 * run out of skbs and will fail and eventually netpoll application such
	 * as netdump will fail.
	 */
	for (i = 0; i < config->tx_fifo_num; i++)
		tx_intr_handler(&mac_control->fifos[i]);

	/* check for received packet and indicate up to network */
	for (i = 0; i < config->rx_ring_num; i++) {
		struct ring_info *ring = &mac_control->rings[i];

		rx_intr_handler(ring, 0);
	}

	for (i = 0; i < config->rx_ring_num; i++) {
		struct ring_info *ring = &mac_control->rings[i];

		if (fill_rx_buffers(nic, ring, 0) == -ENOMEM) {
			DBG_PRINT(INFO_DBG,
				  "%s: Out of memory in Rx Netpoll!!\n",
				  dev->name);
			break;
		}
	}
	enable_irq(dev->irq);
}
#endif

/**
 *  rx_intr_handler - Rx interrupt handler
 *  @ring_info: per ring structure.
 *  @budget: budget for napi processing.
 *  Description:
 *  If the interrupt is because of a received frame or if the
 *  receive ring contains fresh as yet un-processed frames,this function is
 *  called. It picks out the RxD at which place the last Rx processing had
 *  stopped and sends the skb to the OSM's Rx handler and then increments
 *  the offset.
 *  Return Value:
 *  No. of napi packets processed.
 */
static int rx_intr_handler(struct ring_info *ring_data, int budget)
{
	int get_block, put_block;
	struct rx_curr_get_info get_info, put_info;
	struct RxD_t *rxdp;
	struct sk_buff *skb;
	int pkt_cnt = 0, napi_pkts = 0;
	int i;
	struct RxD1 *rxdp1;
	struct RxD3 *rxdp3;

	get_info = ring_data->rx_curr_get_info;
	get_block = get_info.block_index;
	memcpy(&put_info, &ring_data->rx_curr_put_info, sizeof(put_info));
	put_block = put_info.block_index;
	rxdp = ring_data->rx_blocks[get_block].rxds[get_info.offset].virt_addr;

	while (RXD_IS_UP2DT(rxdp)) {
		/*
		 * If your are next to put index then it's
		 * FIFO full condition
		 */
		if ((get_block == put_block) &&
		    (get_info.offset + 1) == put_info.offset) {
			DBG_PRINT(INTR_DBG, "%s: Ring Full\n",
				  ring_data->dev->name);
			break;
		}
		skb = (struct sk_buff *)((unsigned long)rxdp->Host_Control);
		if (skb == NULL) {
			DBG_PRINT(ERR_DBG, "%s: NULL skb in Rx Intr\n",
				  ring_data->dev->name);
			return 0;
		}
		if (ring_data->rxd_mode == RXD_MODE_1) {
			rxdp1 = (struct RxD1 *)rxdp;
			pci_unmap_single(ring_data->pdev, (dma_addr_t)
					 rxdp1->Buffer0_ptr,
					 ring_data->mtu +
					 HEADER_ETHERNET_II_802_3_SIZE +
					 HEADER_802_2_SIZE +
					 HEADER_SNAP_SIZE,
					 PCI_DMA_FROMDEVICE);
		} else if (ring_data->rxd_mode == RXD_MODE_3B) {
			rxdp3 = (struct RxD3 *)rxdp;
			pci_dma_sync_single_for_cpu(ring_data->pdev,
						    (dma_addr_t)rxdp3->Buffer0_ptr,
						    BUF0_LEN,
						    PCI_DMA_FROMDEVICE);
			pci_unmap_single(ring_data->pdev,
					 (dma_addr_t)rxdp3->Buffer2_ptr,
					 ring_data->mtu + 4,
					 PCI_DMA_FROMDEVICE);
		}
		prefetch(skb->data);
		rx_osm_handler(ring_data, rxdp);
		get_info.offset++;
		ring_data->rx_curr_get_info.offset = get_info.offset;
		rxdp = ring_data->rx_blocks[get_block].
			rxds[get_info.offset].virt_addr;
		if (get_info.offset == rxd_count[ring_data->rxd_mode]) {
			get_info.offset = 0;
			ring_data->rx_curr_get_info.offset = get_info.offset;
			get_block++;
			if (get_block == ring_data->block_count)
				get_block = 0;
			ring_data->rx_curr_get_info.block_index = get_block;
			rxdp = ring_data->rx_blocks[get_block].block_virt_addr;
		}

		if (ring_data->nic->config.napi) {
			budget--;
			napi_pkts++;
			if (!budget)
				break;
		}
		pkt_cnt++;
		if ((indicate_max_pkts) && (pkt_cnt > indicate_max_pkts))
			break;
	}
	if (ring_data->lro) {
		/* Clear all LRO sessions before exiting */
		for (i = 0; i < MAX_LRO_SESSIONS; i++) {
			struct lro *lro = &ring_data->lro0_n[i];
			if (lro->in_use) {
				update_L3L4_header(ring_data->nic, lro);
				queue_rx_frame(lro->parent, lro->vlan_tag);
				clear_lro_session(lro);
			}
		}
	}
	return napi_pkts;
}

/**
 *  tx_intr_handler - Transmit interrupt handler
 *  @nic : device private variable
 *  Description:
 *  If an interrupt was raised to indicate DMA complete of the
 *  Tx packet, this function is called. It identifies the last TxD
 *  whose buffer was freed and frees all skbs whose data have already
 *  DMA'ed into the NICs internal memory.
 *  Return Value:
 *  NONE
 */

static void tx_intr_handler(struct fifo_info *fifo_data)
{
	struct s2io_nic *nic = fifo_data->nic;
	struct tx_curr_get_info get_info, put_info;
	struct sk_buff *skb = NULL;
	struct TxD *txdlp;
	int pkt_cnt = 0;
	unsigned long flags = 0;
	u8 err_mask;
	struct stat_block *stats = nic->mac_control.stats_info;
	struct swStat *swstats = &stats->sw_stat;

	if (!spin_trylock_irqsave(&fifo_data->tx_lock, flags))
		return;

	get_info = fifo_data->tx_curr_get_info;
	memcpy(&put_info, &fifo_data->tx_curr_put_info, sizeof(put_info));
	txdlp = (struct TxD *)
		fifo_data->list_info[get_info.offset].list_virt_addr;
	while ((!(txdlp->Control_1 & TXD_LIST_OWN_XENA)) &&
	       (get_info.offset != put_info.offset) &&
	       (txdlp->Host_Control)) {
		/* Check for TxD errors */
		if (txdlp->Control_1 & TXD_T_CODE) {
			unsigned long long err;
			err = txdlp->Control_1 & TXD_T_CODE;
			if (err & 0x1) {
				swstats->parity_err_cnt++;
			}

			/* update t_code statistics */
			err_mask = err >> 48;
			switch (err_mask) {
			case 2:
				swstats->tx_buf_abort_cnt++;
				break;

			case 3:
				swstats->tx_desc_abort_cnt++;
				break;

			case 7:
				swstats->tx_parity_err_cnt++;
				break;

			case 10:
				swstats->tx_link_loss_cnt++;
				break;

			case 15:
				swstats->tx_list_proc_err_cnt++;
				break;
			}
		}

		skb = s2io_txdl_getskb(fifo_data, txdlp, get_info.offset);
		if (skb == NULL) {
			spin_unlock_irqrestore(&fifo_data->tx_lock, flags);
			DBG_PRINT(ERR_DBG, "%s: NULL skb in Tx Free Intr\n",
				  __func__);
			return;
		}
		pkt_cnt++;

		/* Updating the statistics block */
		swstats->mem_freed += skb->truesize;
		dev_kfree_skb_irq(skb);

		get_info.offset++;
		if (get_info.offset == get_info.fifo_len + 1)
			get_info.offset = 0;
		txdlp = (struct TxD *)
			fifo_data->list_info[get_info.offset].list_virt_addr;
		fifo_data->tx_curr_get_info.offset = get_info.offset;
	}

	s2io_wake_tx_queue(fifo_data, pkt_cnt, nic->config.multiq);

	spin_unlock_irqrestore(&fifo_data->tx_lock, flags);
}

/**
 *  s2io_mdio_write - Function to write in to MDIO registers
 *  @mmd_type : MMD type value (PMA/PMD/WIS/PCS/PHYXS)
 *  @addr     : address value
 *  @value    : data value
 *  @dev      : pointer to net_device structure
 *  Description:
 *  This function is used to write values to the MDIO registers
 *  NONE
 */
static void s2io_mdio_write(u32 mmd_type, u64 addr, u16 value,
			    struct net_device *dev)
{
	u64 val64;
	struct s2io_nic *sp = netdev_priv(dev);
	struct XENA_dev_config __iomem *bar0 = sp->bar0;

	/* address transaction */
	val64 = MDIO_MMD_INDX_ADDR(addr) |
		MDIO_MMD_DEV_ADDR(mmd_type) |
		MDIO_MMS_PRT_ADDR(0x0);
	writeq(val64, &bar0->mdio_control);
	val64 = val64 | MDIO_CTRL_START_TRANS(0xE);
	writeq(val64, &bar0->mdio_control);
	udelay(100);

	/* Data transaction */
	val64 = MDIO_MMD_INDX_ADDR(addr) |
		MDIO_MMD_DEV_ADDR(mmd_type) |
		MDIO_MMS_PRT_ADDR(0x0) |
		MDIO_MDIO_DATA(value) |
		MDIO_OP(MDIO_OP_WRITE_TRANS);
	writeq(val64, &bar0->mdio_control);
	val64 = val64 | MDIO_CTRL_START_TRANS(0xE);
	writeq(val64, &bar0->mdio_control);
	udelay(100);

	val64 = MDIO_MMD_INDX_ADDR(addr) |
		MDIO_MMD_DEV_ADDR(mmd_type) |
		MDIO_MMS_PRT_ADDR(0x0) |
		MDIO_OP(MDIO_OP_READ_TRANS);
	writeq(val64, &bar0->mdio_control);
	val64 = val64 | MDIO_CTRL_START_TRANS(0xE);
	writeq(val64, &bar0->mdio_control);
	udelay(100);
}

/**
 *  s2io_mdio_read - Function to write in to MDIO registers
 *  @mmd_type : MMD type value (PMA/PMD/WIS/PCS/PHYXS)
 *  @addr     : address value
 *  @dev      : pointer to net_device structure
 *  Description:
 *  This function is used to read values to the MDIO registers
 *  NONE
 */
static u64 s2io_mdio_read(u32 mmd_type, u64 addr, struct net_device *dev)
{
	u64 val64 = 0x0;
	u64 rval64 = 0x0;
	struct s2io_nic *sp = netdev_priv(dev);
	struct XENA_dev_config __iomem *bar0 = sp->bar0;

	/* address transaction */
	val64 = val64 | (MDIO_MMD_INDX_ADDR(addr)
			 | MDIO_MMD_DEV_ADDR(mmd_type)
			 | MDIO_MMS_PRT_ADDR(0x0));
	writeq(val64, &bar0->mdio_control);
	val64 = val64 | MDIO_CTRL_START_TRANS(0xE);
	writeq(val64, &bar0->mdio_control);
	udelay(100);

	/* Data transaction */
	val64 = MDIO_MMD_INDX_ADDR(addr) |
		MDIO_MMD_DEV_ADDR(mmd_type) |
		MDIO_MMS_PRT_ADDR(0x0) |
		MDIO_OP(MDIO_OP_READ_TRANS);
	writeq(val64, &bar0->mdio_control);
	val64 = val64 | MDIO_CTRL_START_TRANS(0xE);
	writeq(val64, &bar0->mdio_control);
	udelay(100);

	/* Read the value from regs */
	rval64 = readq(&bar0->mdio_control);
	rval64 = rval64 & 0xFFFF0000;
	rval64 = rval64 >> 16;
	return rval64;
}

/**
 *  s2io_chk_xpak_counter - Function to check the status of the xpak counters
 *  @counter      : counter value to be updated
 *  @flag         : flag to indicate the status
 *  @type         : counter type
 *  Description:
 *  This function is to check the status of the xpak counters value
 *  NONE
 */

static void s2io_chk_xpak_counter(u64 *counter, u64 * regs_stat, u32 index,
				  u16 flag, u16 type)
{
	u64 mask = 0x3;
	u64 val64;
	int i;
	for (i = 0; i < index; i++)
		mask = mask << 0x2;

	if (flag > 0) {
		*counter = *counter + 1;
		val64 = *regs_stat & mask;
		val64 = val64 >> (index * 0x2);
		val64 = val64 + 1;
		if (val64 == 3) {
			switch (type) {
			case 1:
				DBG_PRINT(ERR_DBG,
					  "Take Xframe NIC out of service.\n");
				DBG_PRINT(ERR_DBG,
"Excessive temperatures may result in premature transceiver failure.\n");
				break;
			case 2:
				DBG_PRINT(ERR_DBG,
					  "Take Xframe NIC out of service.\n");
				DBG_PRINT(ERR_DBG,
"Excessive bias currents may indicate imminent laser diode failure.\n");
				break;
			case 3:
				DBG_PRINT(ERR_DBG,
					  "Take Xframe NIC out of service.\n");
				DBG_PRINT(ERR_DBG,
"Excessive laser output power may saturate far-end receiver.\n");
				break;
			default:
				DBG_PRINT(ERR_DBG,
					  "Incorrect XPAK Alarm type\n");
			}
			val64 = 0x0;
		}
		val64 = val64 << (index * 0x2);
		*regs_stat = (*regs_stat & (~mask)) | (val64);

	} else {
		*regs_stat = *regs_stat & (~mask);
	}
}

/**
 *  s2io_updt_xpak_counter - Function to update the xpak counters
 *  @dev         : pointer to net_device struct
 *  Description:
 *  This function is to upate the status of the xpak counters value
 *  NONE
 */
static void s2io_updt_xpak_counter(struct net_device *dev)
{
	u16 flag  = 0x0;
	u16 type  = 0x0;
	u16 val16 = 0x0;
	u64 val64 = 0x0;
	u64 addr  = 0x0;

	struct s2io_nic *sp = netdev_priv(dev);
	struct stat_block *stats = sp->mac_control.stats_info;
	struct xpakStat *xstats = &stats->xpak_stat;

	/* Check the communication with the MDIO slave */
	addr = MDIO_CTRL1;
	val64 = 0x0;
	val64 = s2io_mdio_read(MDIO_MMD_PMAPMD, addr, dev);
	if ((val64 == 0xFFFF) || (val64 == 0x0000)) {
		DBG_PRINT(ERR_DBG,
			  "ERR: MDIO slave access failed - Returned %llx\n",
			  (unsigned long long)val64);
		return;
	}

	/* Check for the expected value of control reg 1 */
	if (val64 != MDIO_CTRL1_SPEED10G) {
		DBG_PRINT(ERR_DBG, "Incorrect value at PMA address 0x0000 - "
			  "Returned: %llx- Expected: 0x%x\n",
			  (unsigned long long)val64, MDIO_CTRL1_SPEED10G);
		return;
	}

	/* Loading the DOM register to MDIO register */
	addr = 0xA100;
	s2io_mdio_write(MDIO_MMD_PMAPMD, addr, val16, dev);
	val64 = s2io_mdio_read(MDIO_MMD_PMAPMD, addr, dev);

	/* Reading the Alarm flags */
	addr = 0xA070;
	val64 = 0x0;
	val64 = s2io_mdio_read(MDIO_MMD_PMAPMD, addr, dev);

	flag = CHECKBIT(val64, 0x7);
	type = 1;
	s2io_chk_xpak_counter(&xstats->alarm_transceiver_temp_high,
			      &xstats->xpak_regs_stat,
			      0x0, flag, type);

	if (CHECKBIT(val64, 0x6))
		xstats->alarm_transceiver_temp_low++;

	flag = CHECKBIT(val64, 0x3);
	type = 2;
	s2io_chk_xpak_counter(&xstats->alarm_laser_bias_current_high,
			      &xstats->xpak_regs_stat,
			      0x2, flag, type);

	if (CHECKBIT(val64, 0x2))
		xstats->alarm_laser_bias_current_low++;

	flag = CHECKBIT(val64, 0x1);
	type = 3;
	s2io_chk_xpak_counter(&xstats->alarm_laser_output_power_high,
			      &xstats->xpak_regs_stat,
			      0x4, flag, type);

	if (CHECKBIT(val64, 0x0))
		xstats->alarm_laser_output_power_low++;

	/* Reading the Warning flags */
	addr = 0xA074;
	val64 = 0x0;
	val64 = s2io_mdio_read(MDIO_MMD_PMAPMD, addr, dev);

	if (CHECKBIT(val64, 0x7))
		xstats->warn_transceiver_temp_high++;

	if (CHECKBIT(val64, 0x6))
		xstats->warn_transceiver_temp_low++;

	if (CHECKBIT(val64, 0x3))
		xstats->warn_laser_bias_current_high++;

	if (CHECKBIT(val64, 0x2))
		xstats->w