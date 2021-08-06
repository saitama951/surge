# surge
**Testing**

The logs are present in the logs folder inside tests which contains the testing for different parameters in the given local and 
remote test code which is divided by modules. 

each log file is named after { log(local test code module number)(remote test code module number) }
example:- log21 means the second module of the local process is executed and the first module of the remote process is executed



my kernel version
--------------------
```5.4.123-1-MANJARO```

steps to run
-----------
1. ```make```
2. ```sudo insmod```
3. ```gcc phy_addr_app.c```
4. ```sudo ./a.out```
5. ```sudo dmesg```
6. ```sudo rmmod```

