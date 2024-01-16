# ECE438 Homework 1 Jiadong Hong

## Section 1

### Q1: ABD

![image-20231017120825227](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231017120825227.png)

### Q2: A

![image-20231017121242305](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231017121242305.png)

### Q3: B; E

![image-20231017121931416](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231017121931416.png)

### Q4: C

![image-20231017122414358](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231017122414358.png)

### Q5: ABD

![image-20231017125640897](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231017125640897.png)

### Q6: B; C

![image-20231017125913435](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231017125913435.png)

## Section 2

### Suppose that 3 users are sharing a 300 Mbps connection. Each user uses the link 20% of the time. Assume their internet access activity is independent from each other and the network use is distributed uniformly.

#### 1. What is the probability that no user is using the link simultaneous at the given time?

Given that each user uses the link 20% of the time, the probability that a user is not using the link at a given time is 1 - 0.20 = 0.80 (80%). Since the activities of the three users are independent, you can multiply their individual probabilities to find the probability that none of them is using the link simultaneously.

Probability (no user is using the link) = (0.80) * (0.80) * (0.80) = 0.512 (51.2%)

#### 2. What is the probability that two users are using the link simultaneous at the given time?

To calculate the probability that two users are using the link at the same time, you need to consider the combination of two users from the three.

Probability (two users are using the link) = C(3, 2) * (0.20)^2 * (0.80) = 3 * 0.04 * 0.80 = 0.096 (9.6%)

Here, C(3, 2) represents the combination of choosing 2 out of 3 users.

#### 3. Suppose that you want to use the link. Assume that when two or more people use the bandwidth are divided fairly among them. What is the average bandwidth you will receive?

When you use the link and it's divided fairly among all users, you will get 1/3 of the total bandwidth. This is because there are three users sharing a 300 Mbps connection. So, you will receive 1/3 * 300 Mbps = 100 Mbps on average when you use the link.

## Section 3

### Question 1:  Explain the difference between transmission delay and propagation delay.

Transmission delay is related to how quickly data can be pushed onto a communication link, and it depends on the size of the data and the link's bandwidth. Propagation delay, on the other hand, is about the time it takes for data to travel through the physical medium and is determined by the distance and the speed of signal propagation in that medium.

### Question 2: Suppose a router processes packets at the rate R=1 packet per second. Packets are arriving into the router’s queue at time ticks (in seconds) shown in the table below. Compute 

![image-20231017134457512](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231017134457512.png)

#### (A) the average packet throughput in the first 10 seconds

Average Packet Throughput = 10/11 = 0.91 packet per second

#### (B) the average queuing delay. Please only type in the final result in the text box. (Round your answer to 2 decimal places)

delay = 1/10 = 0.10 (s)

![image-20231017134514832](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231017134514832.png)

#### (A) the average packet throughput in the first 10 seconds

Average Packet Throughput = 1 packet per second

#### (B) the average queuing delay. Please only type in the final result in the text box. (Round your answer to 2 decimal places)

delay = (0+1+2+3+0+1+2+3+4+5)/10=2.1(s)

### Consider two hosts, A and B, that are connected by switch S. The link A-S is 100Mbps and has a propagation delay of 10ms. The link B-S is 80Mbps and has a propagation delay of 30ms. (1B = 8 bit, Assume 1KB = 1000B, 1MB = 1000KB)

#### 3. Assume that no processing delay. If A sends a 1MB packet to B, what will the end-to-end delay be?

10ms+30ms+(8x10^5)*(1/100M+1/80M) *10^3 = 58ms

#### 4. Suppose A sends 20 100KB packets to B continuously. Suppose S has a 500KB buffer for packets, will the packet be dropped?

A-S: 8ms+10ms

S-B: 10ms+30ms

take 0-360ms as example:

S-B: 360/40 = 9 packets

A-S: 360/18 = 20 packets

Then we can figure out even the buffer is full at this time point, there is at least 6 packets been dropped.

## Section 4

### Shannon’s ground breaking equation says that: C = Blog2 (1 + SNR) where C is the data rate in bits/s achievable on the communication link (also called capacity), B is the bandwidth in Hz, and SNR is the ratio of received signal power to the receiver’s noise power. Assume that the received signal power density P ∝ 1/R2 where R is the distance between sender and receiver.

#### 1. Suppose a laptop tends to transmit to a WIFI station located 10m away. Assume signal power density measured 2 meters from the laptop is Q = 12milliW att/m2 and the noise power density at the receiver is N = 0.01milliW att/m2 . Suppose the laptop transmits at a bandwidth of 20MHz, what data rate can it achieve? (round your answer to 3 decimal places)

P_10m = Q / (10m/2m)² = 12 mW/m² / 25 = 0.48 mW/m² 

SNR = P / N = 0.48 mW/m² / 0.01 mW/m² = 48 

C = B×log₂(1 + SNR) = 20 × 106 × log₂(1 + 48) = 112.294 Mbps

#### 2. List at least two methods that can increase the data rate.

Increase Bandwidth

Enhance the Signal-to-Noise Ratio (SNR)

#### 3. If the laptop intends to triple its data rate, how close should it move to the WiFi station? Assume all the other conditions are the same. (round your answer to 3 decimal places)

P_R2 = Q / (R2/2)^2  

C_R2 = B×log₂(1 + P_R2/N)  

Thus, we can solve for R2 given that C_R2 = 3 × C_R1, R2 = 0.202m

## Section 5

### Mark all statements that are correct based on the classical principles of the network protocol stack: (-2pt per option wrong until 0) 

(a) HTTP, SMTP, FTP are application layer protocols. 

(b) All Internet components that have a network layer must run the IP protocol. 

(c) The Transportation layer header can be read and modified by the router. 

(d) The Network layer header can be read and modified by the router. 

(e) Transport, Network, Link, and Physical layers are implemented at the core routers 

(f) Suppose you send an email to your friend in another country, and your packets’ link layer header does not contain your friend’s link-layer address. 

(g) Reducing the size of headers improves the goodput of the network 

#### (a)(b)(d)

### Briefly answer the following questions. 

#### (a) List at least one advantage and disadvantage of protocol layering. 

**Advantage of Protocol Layering:**

- **Modularity:** Protocol layering allows for a modular and organized approach to network design. Each layer can be designed, implemented, and maintained separately, making it easier to upgrade or replace individual components without affecting the entire system.

**Disadvantage of Protocol Layering:**

- **Overhead:** Layering can introduce overhead in terms of additional headers and processing at each layer, which can impact the efficiency and performance of the network.

#### (b) Why will two ISPs at the same level of the hierarchy often peer with each other? How does an IXP earn money?

**Why ISPs Peer and How an IXP Earns Money:**

- **ISPs Peer:** ISPs at the same level of the hierarchy often peer with each other to exchange traffic directly, improving network efficiency and reducing the cost of data transit. This peering allows them to share traffic without having to route it through a higher-tier ISP, which can be costly. It also improves the user experience by reducing latency and increasing network resilience.
- **IXP (Internet Exchange Point) Earnings:** IXPs typically earn money through membership fees and port charges. ISPs and other network providers pay fees to become members of the IXP, and they also pay for the ports or connections they use to connect to the IXP's infrastructure. This revenue is used to maintain and expand the IXP's facilities, ensuring efficient traffic exchange between members.