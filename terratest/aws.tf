variable "amis" {
  default = {
    us-east-1      = "ami-6d1c2007"
    us-west-2      = "ami-d2c924b2"
    us-west-1      = "ami-af4333cf"
    eu-central-1   = "ami-9bf712f4"
    eu-west-1      = "ami-7abd0209"
    ap-southeast-1 = "ami-f068a193"
    ap-southeast-2 = "ami-fedafc9d"
    ap-northeast-1 = "ami-eec1c380"
    sa-east-1      = "ami-26b93b4a"
  }
}
variable "availability_zones"  {
  default = "a,b,c"
}
variable "control_count" { default = 1 }
variable "datacenter" {default = "aws-us-west-2"}
variable "edge_count" { default = 2 }
variable "region" {default = "us-west-2"}
variable "short_name" {default = "mantl"}
variable "long_name" {default = "mantl"}
variable "ssh_username" {default = "centos"}
variable "worker_count" { default = 4 }
variable "kubeworker_count" { default = 2 }
variable "dns_subdomain" { default = ".dev" }
variable "dns_domain" { default = "hongqi.com." }
variable "dns_zone_id" { default = "Z15OHQVEEQ76RR" }
variable "control_type" { default = "m3.medium" }
variable "edge_type" { default = "m3.medium" }
variable "worker_type" { default = "m3.large" }
variable "kubeworker_type" { default = "m3.large" }
variable "role" { default = "control"}
variable "count_format" {default = "%02d"}
variable "ebs_volume_size" {default = "20"} # size is in gigabytes
variable "ebs_volume_type" {default = "gp2"}
provider "aws" {
  region = "${var.region}"
}
# Create a subnet to launch our instances into
resource "aws_subnet" "default" {
  vpc_id                  = "${aws_vpc.default.id}"
  cidr_block              = "10.0.1.0/24"
  map_public_ip_on_launch = true
}
resource "aws_vpc" "default" {
  cidr_block = "10.0.0.0/16"
}

#module "vpc" {
#  source ="./terraform/aws/vpc"
#  availability_zones = "${var.availability_zones}"
#  short_name = "${var.short_name}"
#  long_name = "${var.long_name}"
#  region = "${var.region}"
#}

#module "ssh-key" {
#  source ="./terraform/aws/ssh"
#  short_name = "${var.short_name}"
#}

#module "security-groups" {
#  source = "./terraform/aws/security_groups"
#  short_name = "${var.short_name}"
#  vpc_id = "${module.vpc.vpc_id}"
#}

#module "iam-profiles" {
#  source = "./terraform/aws/iam"
#  short_name = "${var.short_name}"
#}

resource "aws_instance" "control_node" {
  ami = "${lookup(var.amis, var.region)}"
  instance_type = "${var.control_type}"
  count = "${var.control_count}"
  #vpc_security_group_ids = [ "${split(",", "${module.vpc.default_security_group},${module.security-groups.ui_security_group},${module.security-groups.control_security_group}")}"]
  #key_name = "${module.ssh-key.ssh_key_name}"
  associate_public_ip_address = true
  #subnet_id = "${element(split(",", "${module.vpc.subnet_ids}"), count.index)}"
  subnet_id = "${aws_subnet.default.id}"
  
  #iam_instance_profile = "${module.iam-profiles.control_iam_instance_profile}"
  root_block_device {
    delete_on_termination = true
    volume_size = "${var.ebs_volume_size}"
    volume_type = "${var.ebs_volume_type}"
  }


  tags {
    Name = "${var.short_name}-${var.role}-${format(var.count_format, count.index+1)}"
    sshUser = "${var.ssh_username}"
    role = "control"
    dc = "${var.datacenter}"
    KubernetesCluster = "${var.short_name}"
  }
}

module "route53" {
  source = "./terraform/aws/route53/dns"
  control_count = "${var.control_count}"
  control_ips = "${aws_instance.control_node.public_ip}"
  domain = "${var.dns_domain}"
  edge_count = "${var.edge_count}"
  hosted_zone_id = "${var.dns_zone_id}"
  short_name = "${var.short_name}"
  subdomain = "${var.dns_subdomain}"
  worker_count = "${var.worker_count}"
  kubeworker_count = "${var.kubeworker_count}"
}
