variable "size" {
  type = string
  default = "Standard_B4ms"
}

# variable "rg" {
#   type = object({
#     name = string
#     location = string
#   })
# }

variable "location" {
  type = string
  default = "West Europe"
}