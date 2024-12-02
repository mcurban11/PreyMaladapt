resource "azurerm_linux_virtual_machine" "vm-tiny-server" {
  name                = "vm-tiny-server"
  # resource_group_name = var.rg.name
  # location            = var.rg.location
  resource_group_name = azurerm_resource_group.rg-tiny.name
  location            = azurerm_resource_group.rg-tiny.location
  size                = var.size
  admin_username      = "tiny"
  network_interface_ids = [
    azurerm_network_interface.nic-tiny-server.id,
  ]

  admin_ssh_key {
    username   = "tiny"
    public_key = file("~/.ssh/aws-personal.pub")
  }

  os_disk {
    caching              = "ReadWrite"
    storage_account_type = "Premium_LRS"
  }

  source_image_reference {
    publisher = "Canonical"
    offer     = "0001-com-ubuntu-server-jammy"
    sku       = "22_04-lts-gen2"
    version   = "latest"
  }
}