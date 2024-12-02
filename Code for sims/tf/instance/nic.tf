resource "azurerm_network_interface" "nic-tiny-server" {
  name                = "nic-tiny-server"
  # location            = var.rg.location
  # resource_group_name = var.rg.name
  resource_group_name = azurerm_resource_group.rg-tiny.name
  location            = azurerm_resource_group.rg-tiny.location

  ip_configuration {
    name                          = "ip-tiny-server"
    subnet_id                     = azurerm_subnet.subnet-tiny-server.id
    private_ip_address_allocation = "Dynamic"
    public_ip_address_id = azurerm_public_ip.ip-tiny-server.id
  }
}
