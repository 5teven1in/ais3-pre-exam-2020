FROM ubuntu:18.04
MAINTAINER ss8651twtw <ss8651twtw@gmail.com>
RUN apt-get -qq update
RUN apt-get -qqy install xinetd
RUN useradd -m portal_gun
RUN chmod 774 /tmp
RUN chmod -R 774 /var/tmp
RUN chmod -R 774 /dev
RUN chmod -R 774 /run
RUN chmod 1733 /tmp /var/tmp /dev/shm
RUN chown -R root:root /home/portal_gun
CMD ["/usr/sbin/xinetd","-dontfork"]
