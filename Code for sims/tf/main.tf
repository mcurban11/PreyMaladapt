terraform {
  required_providers {
    azurerm = {
      source  = "hashicorp/azurerm"
      version = "=3.0.0"
    }
  }
}

provider "azurerm" {
  features {}

  subscription_id = "551273ef-15c1-4c79-a7d8-452c38380259"
  tenant_id       = "46cbc9d7-da4c-46a4-916a-47e50799fad7"
}

module "vm-1" {
  source = "./instance"
  location = "uksouth"

  # size = var.vm-size
  size =  "Standard_F4s_v2"
  # rg = {
  #   name     = resource.azurerm_resource_group.rg-tiny.name
  #   location = resource.azurerm_resource_group.rg-tiny.location
  # }
}

# module "vm-2" {
#   source = "./instance"
#   location = "westeurope"

#   # size = var.vm-size-2
#   size =  "Standard_F4s_v2"
#   # rg = {
#   #   name     = resource.azurerm_resource_group.rg-tiny.name
#   #   location = resource.azurerm_resource_group.rg-tiny.location
#   # }
# }

# module "vm-3" {
#   source = "./instance"
#   location = "northeurope"

#   # size = var.vm-size
#   size =  "Standard_F4s_v2"
#   # rg = {
#   #   name     = resource.azurerm_resource_group.rg-tiny.name
#   #   location = resource.azurerm_resource_group.rg-tiny.location
#   # }
# }