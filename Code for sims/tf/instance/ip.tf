resource "azurerm_public_ip" "ip-tiny-server" {
  name                = "ip-tiny-server"
  # resource_group_name = var.rg.name
  # location            = var.rg.location
  resource_group_name = azurerm_resource_group.rg-tiny.name
  location            = azurerm_resource_group.rg-tiny.location
  allocation_method   = "Static"
}