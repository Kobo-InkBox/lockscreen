#!/bin/bash
echo what

mv lockscreen exec | EXIT_CODE=0

#aaaaaservername="root@10.42.0.28"
passwd="root"
servername="root@192.168.2.2"

sshpass -p $passwd ssh $servername "bash -c \"ifsctl mnt rootfs rw\""
sshpass -p $passwd ssh $servername "bash -c \"rm /kobo/tmp/exec\""
sshpass -p $passwd scp exec $servername:/kobo/tmp/

sshpass -p $passwd ssh $servername "bash -c \"sync\""

sshpass -p $passwd ssh $servername "bash -c \"/kobo/launch_lock.sh\""
