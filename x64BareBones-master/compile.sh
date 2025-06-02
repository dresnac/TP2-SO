sudo docker start TP2_SO
sudo docker exec -it TP2_SO make clean -C /root/Toolchain
sudo docker exec -it TP2_SO make clean -C /root/
sudo docker exec -it TP2_SO make -C /root/Toolchain
sudo docker exec -it TP2_SO make -C /root/
sudo docker stop TP2_SO
sudo chmod 777 Image/x64BareBonesImage.qcow2