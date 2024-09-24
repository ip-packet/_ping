# *`S_PING`*
## about
a lightweight utility designed to emulate a simplified version of the ping command. It provides basic network connectivity testing by sending ICMP echo requests to a specified host and displaying response times.
While offering a core functionality, s_ping may have limitations compared to the full-featured ping command, such as missing advanced options or detailed statistics.
## requirements
- linux
- root privilege
## usage
```
./s_ping DEST [timeout]
  DEST: domain name/ipv4 address of the target host
  timeout: optional number of seconds until host response
```
## example output
```
$ ./s_ping 1337.ma
1337.ma (104.18.32.248)
	icmp_echo 104.18.32.248 : icmp_echoreply t=55.701 ms, echo.id=13830, icmp_seq=0
	icmp_echo 104.18.32.248 : icmp_echoreply t=58.195 ms, echo.id=13830, icmp_seq=1
	icmp_echo 104.18.32.248 : icmp_echoreply t=32.952 ms, echo.id=13830, icmp_seq=2
	icmp_echo 104.18.32.248 : icmp_echoreply t=20.171 ms, echo.id=13830, icmp_seq=3
	icmp_echo 104.18.32.248 : icmp_echoreply t=27.417 ms, echo.id=13830, icmp_seq=4
	icmp_echo 104.18.32.248 : icmp_echoreply t=21.416 ms, echo.id=13830, icmp_seq=5
^Cquiting..
=== 104.18.32.248 stats ===
packets sent: 7, packet received: 7
time: 1230.085 ms
```
