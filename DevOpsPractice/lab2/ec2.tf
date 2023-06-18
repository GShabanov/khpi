module "ec2_type1" {
  source                      = "./ec2"
  basename                    = "instance-type1"
  environment                 = "demo"
  ami                         = data.aws_ami.ubuntu-linux.id
  vpc_id                      = aws_vpc.vpc.id
  security_group_ids          = [aws_security_group.main_sg.id]
  subnet_id                   = aws_subnet.subnet-public.id
  instance_type               = "t2.micro"
  associate_public_ip_address = true
  instance_count              = (["1"])
  private_key_path            = var.private-key-path
  remote_exec_script          = "./first_instance.sh"
}

module "ec2_type2" {
  source                      = "./ec2"
  basename                    = "instance-type2"
  environment                 = "demo"
  ami                         = data.aws_ami.ubuntu-linux.id
  vpc_id                      = aws_vpc.vpc.id
  security_group_ids          = [aws_security_group.main_sg.id]
  subnet_id                   = aws_subnet.subnet-public.id
  instance_type               = "t2.micro"
  associate_public_ip_address = true
  instance_count              = (["1"])
  private_key_path            = var.private-key-path
  remote_exec_script          = "./second_instance.sh"
}