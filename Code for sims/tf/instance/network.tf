resource "azurerm_virtual_network" "net-tiny-server" {
  name                = "net-tiny-server"
  address_space       = ["10.0.0.0/16"]
  # resource_group_name = var.rg.name
  # location            = var.rg.location
  resource_group_name = azurerm_resource_group.rg-tiny.name
  location            = azurerm_resource_group.rg-tiny.location
}

resource "azurerm_subnet" "subnet-tiny-server" {
  name                 = "subnet-tiny-server"
  # resource_group_name  = var.rg.name
  resource_group_name = azurerm_resource_group.rg-tiny.name
  virtual_network_name = azurerm_virtual_network.net-tiny-server.name
  address_prefixes     = ["10.0.2.0/24"]
}
