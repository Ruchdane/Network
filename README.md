# Network

A tcp client and server command line utilities that can be used to [send](./src/send.c) and [receive](./src/receive.c) data over nework without any data cost

## Send

```
Usage : send  [-s] [-p  <Port>]  <File Path>
 Options:
  -s secure upload
  -p set alternative port to use

```
## Receive

```
 Usage : receive  [-s] [-p  <Port>] [-d <destination path>] <IP address> 
 Options:
  -s secure download
  -p set alternative port used by the sender
  -d set destination directory


```
The ip adress is shown on the send execution
### Build

It comes with a makefile so to build you just use the command 
```
make send receive
```

## Contact
[Email](ruchdaneabiodun@gmail.com)
