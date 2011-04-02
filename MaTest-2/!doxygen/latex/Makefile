	return err;
	}

	mod_timer(&qdev->timer, jiffies + (5*HZ));
	netif_device_attach(ndev);

	return 0;
}
#endif /* CONFIG_PM */

static void qlge_shutdown(struct pci_dev *pdev)
{
	qlge_suspend(pdev, PMSG_SUSPEND);
}

static struct pci_driver qlge_driver = {
	.name = DRV_NAME,
	.id_table = qlge_pci_tbl,
	.probe = qlge_probe,
	.remove = __devexit_p(qlge_remove),
#ifdef CONFIG_PM
	.suspend = qlge_suspend,
	.resume = qlge_resume,
#endif
	.shutdown = qlge_shutdown,
