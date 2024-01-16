# ECE438 Homework 3 Jiadong Hong

## Part1

![image-20231124213130580](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231124213130580.png)

A

A TCP socket is an end-to-end connection between processes.![image-20231124213155982](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231124213155982.png)

B

The acknowledgement number indicates the next expected sequence number to receive. So if Host A sends a sequence number of 40, and the data length is 8 bytes, then the next expected sequence number Host B should receive is 40 + 8 = 48. Therefore, the correct answer is (a) True.

![image-20231124213216650](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231124213216650.png)

CD

In the Selective ACK protocol, the receiver's base sequence number can be larger than the transmitter's tail sequence number. So the correct answers are (c) and (d).

## Part 2

![image-20231124222953260](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231124222953260.png)

(1) CW at time t1 = 2

(2) CW at time t2 = 4

(3) CW at time t3 = 4

(4)  How should the TCP transmitter react after receiving A3? Please give CW head, CW tail, and Send.

CW increase by 2

Send P4-7 since ACK3 indicates 3 has arrived.

CW_Head = 4, CW_tail = 7, Send = [4,5,6,7]

(5) How should the TCP transmitter react after receiving A2? Please give CW head, CW tail, and Send

There is no change for CW and there is no package send

CW_Head = 4, CW_tail = 7, Send = []

## Part 3

![image-20231127224525901](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231127224525901.png)

(1) CW = 3

(2) CW = 1

(3) CW = 1

(4) Resend P3, CW_head = 3, CW_tail = 3, SS_thresh = 1.5, send = [3]

(5) no action, CW_head = 3, CW_tail = 3, SS_thresh = 1.5, send = []

(6) no action, CW_head = 3, CW_tail = 3, SS_thresh = 1.5, send = []

## Part 4

![](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231127232403923.png)

(1) CW = 11

(2) CW = 12

(3) CW++; send P24-25, CW_head = 14, CW_tail = 25, send = [24,25]

## Part 5

![image-20231127232713537](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231127232713537.png)

(1) CW = 9

(2) CW = 1

(3) CW would be increased by 1, and the host would send P10-14, CW_head = 6, CW_tail = 14, Send = [10,11,12,13,14]

(4) Stay in slow start state, Resend P6, CW_head = 6, CW_tail = 6, Send = [6], SS_thresh = 4.5

![image-20231127232846847](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231127232846847.png)

(1) t1: CW =  19/3, t2: CW =  20/3  t3: CW =  20/3  t4: CW =  41/6.

(2) CW += 1/6, send P15-16, CW_head = 11, CW_tail = 16, Send = [15,16], SS_thresh = 6

(3) CW += 1/6, Send P17, CW_head = 12, CW_tail = 17, Send = [17], SS_thresh = 6

(4) CW += 1/3, Send P18-19, CW_head = 14, CW_tail = 19, Send = [18, 19], SS_thresh = 6

(5) NO action. CW_head = 14, CW_tail = 19, Send = [], SS_thresh = 6

(6) CW += 1/6, Send P20, CW_head = 15, CW_tail = 20, Send = [20], SS_thresh = 6

![image-20231127234253050](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231127234253050.png)

(1) False, With Selective Repeat (SR) protocol, the sender maintains a window of acceptable sequence numbers for the acknowledgment (ACK) of the received packets. The window represents a range of acceptable sequence numbers, and the sender expects to receive ACKs only for packets within this window.

(2) True. With the Go-Back-N (GBN) protocol, it is possible for the sender to receive an acknowledgment (ACK) for a packet that falls outside of its current window. In GBN, the sender continues to send a sequence of packets without waiting for individual acknowledgments. The receiver, however, can selectively acknowledge correctly received packets.

![image-20231127234551266](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231127234551266.png)

**1. Possible set of sequence numbers inside the sender's window at time t:**
   At time t, the sender's window contains sequence numbers from the current "window-base" (also called "window-head") up to the sequence number of the last packet that has been sent. Therefore, the possible set of sequence numbers inside the sender's window at time t is `[window-base, window-base + N - 1]`. This is because the window size is N, and it covers N consecutive sequence numbers.

**2. Possible values of the ACK field in all possible messages currently propagating back to the sender at time t:**
   At time t, acknowledgments may be in transit for the packets sent earlier. The acknowledgment number in an ACK message represents the next expected sequence number at the receiver. Therefore, the possible values of the ACK field in ACK messages at time t are `[k, k + N - 1]`, where k is the sequence number of the next in-order packet that the receiver is expecting.

   *Justification:* The receiver acknowledges all correctly received packets up to the next expected sequence number. Since the receiver is expecting the next in-order packet with a sequence number of k, the acknowledgment can acknowledge packets up to `k + N - 1` in case all packets within the window have been correctly received.

Please note that if there are dropped or lost packets, the sender may receive duplicate ACKs for the same sequence number, and the sender would then retransmit the unacknowledged packets in its window.