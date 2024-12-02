resource "azurerm_resource_group" "rg-tiny" {
  name     = "rg-tiny-server-${var.location}"
  location = var.location
}