# ECE438 Homework 2 Jiadong Hong

## 1 Choose all that Apply

![image-20231103190105678](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231103190105678.png)

(a) In traditional HTTP/1.0, a separate connection was established for each resource request (e.g., an HTML page, an image, a stylesheet), which could result in a significant overhead due to the repeated connection setup and teardown. However, in HTTP/1.1, which is the most widely used version of HTTP as of my last knowledge update in January 2022, persistent connections are supported by default.

![image-20231103190310554](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231103190310554.png)

(a) Yes.

![image-20231103190411942](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231103190411942.png)

(c)

![image-20231103190640477](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231103190640477.png)

(b)

## 2 Short Answer Questions - 5 × 2 points

### 1. Briefly explain the advantages and disadvantages of the use of cookie

**Advantages of Using Cookies:**

1. **Session Management:** Cookies are commonly used to manage user sessions. They can store session identifiers that allow websites to recognize and remember users as they navigate between pages, which is essential for maintaining user login status and personalized experiences.
2. **Customization:** Cookies enable websites to personalize content and settings for individual users based on their preferences and behavior. This can enhance user experience and engagement.
3. **Tracking and Analytics:** Cookies are used for tracking user behavior on websites. They provide valuable data for web analytics, helping businesses understand how users interact with their sites and make data-driven decisions.
4. **Shopping Carts and E-commerce:** Cookies are essential for e-commerce websites to maintain shopping cart contents and remember user selections. This is crucial for a smooth shopping experience.
5. **Remembering User Preferences:** Cookies can store user preferences, such as language settings or display preferences, allowing websites to remember these choices across visits.

**Disadvantages of Using Cookies:**

1. **Privacy Concerns:** Cookies can raise privacy concerns, as they can track and store user data, potentially infringing on user privacy. Users may be uncomfortable with the amount of data collected.
2. **Security Risks:** Cookies can be vulnerable to various security threats, including cookie theft, session hijacking, and cross-site scripting (XSS) attacks if not implemented securely.
3. **Cross-Site Tracking:** Some users may be concerned about being tracked across different websites by third-party cookies. This can lead to privacy issues and unwanted advertisements.
4. **Limited Data Storage:** Cookies have a size limit (typically 4KB), which restricts the amount of data that can be stored. This can be a limitation for applications that require larger data storage.
5. **Browser Settings:** Users can disable or block cookies in their web browsers, which may disrupt some website functionality, such as login sessions or personalization.

### 2. Describe how Web caching can reduce the delay in receiving a requested object. Will Web caching reduce the delay for all objects requested by a user or for only some of the objects? Explain why.

**How Web Caching Reduces Delay:**

1. **Reduced Round-Trip Time:** When a user's request is served from a cache, it eliminates the need to fetch the object from the original web server. This reduces the round-trip time, as the object is already stored closer to the user, typically in the same geographical region or on the same network.
2. **Faster Retrieval:** Retrieving objects from a cache is faster because it doesn't involve the time-consuming process of establishing a connection with the original server, waiting for the server to process the request, and transferring data over the internet. Instead, the cache server can respond quickly with the cached copy.
3. **Less Congestion:** By serving content from a cache, it reduces the load on the original web server. This helps prevent congestion and overloads on the server, ensuring a more responsive and reliable user experience.

**Web Caching and Objects Requested by a User:**

Web caching reduces the delay for some of the objects requested by a user, but not necessarily for all objects. The extent to which web caching reduces the delay depends on several factors, including:

1. **Cache Hit vs. Cache Miss:** When a user requests an object, the caching server checks if it has a cached copy (cache hit) or if it needs to fetch the object from the original server (cache miss). If the object is in the cache (cache hit), the delay is reduced. If it's not in the cache (cache miss), the delay is not reduced, and the object must be fetched from the original server.
2. **Cache Policies:** Cache servers are configured with policies that determine what objects to cache, how long to keep them, and when to invalidate them. These policies vary, and not all objects may be cached or kept for an extended period. Frequently accessed or popular objects are more likely to be cached.
3. **Cache Size:** Cache servers have limited storage capacity. Less frequently accessed or less popular objects may not be cached due to space limitations. As a result, only a subset of objects is cached.

## 3 Web Caching - 7 x 3 points

### Assume a group of students in an institution want to access a private server A outside of the institution. The bottleneck link from the institution to this server supports a bitrate of 2MB/S. Assume the average request rate from the institution is 80 times/s and each request is 0.02MB. Assuming there is no other traffic within or outside of the institution, answer the following questions. Assume that queueing delay dominates so you can neglect the much smaller propagation delays, transmit times, and processing delays 

#### 1. What is the average access time for a user in the institution to access this server? Assume the queuing delay is 1/(1-L) milliseconds, where L is the fraction of link usage. (Your answer should be in milliseconds).

![b561084697019676d8c71bdc330f198](C:\Users\Jiadong Hong\Documents\WeChat Files\wxid_8u2i17dq6v7v22\FileStorage\Temp\b561084697019676d8c71bdc330f198.jpg)

#### 2. To improve network performance, we now increase the bitrate of this bottleneck link to 6MB/s. Calculate the average access time again. Your unit should be milliseconds and must be computed up to 2 decimal places.

![35545f9b0f1c1b4c118a72fe08db4cc](C:\Users\Jiadong Hong\Documents\WeChat Files\wxid_8u2i17dq6v7v22\FileStorage\Temp\35545f9b0f1c1b4c118a72fe08db4cc.jpg)

#### 3. Another way to improve network performance is to add a cache server within the institution without increasing the bandwidth of bottleneck link. The bitrate to the cache server is 10MB/s. Assume there is a 60% cache hit rate. The queuing delay for both cache server and server A follows the formula in Q1. Calculate the average access time in this case. (Assume the network knows cache server so no additional delays are needed to find that cache server; also, your unit should be milliseconds, computed to 2 decimal places).

![f49ccd9d610cf9820aa4ecd052ea8ff](C:\Users\Jiadong Hong\Documents\WeChat Files\wxid_8u2i17dq6v7v22\FileStorage\Temp\f49ccd9d610cf9820aa4ecd052ea8ff.jpg)

## 4 Traceroute - 4 × 3 points

![image-20231103222951508](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231103222951508.png)

![image-20231103223728424](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231103223728424.png)

### Traceroute to [www.google.com](http://www.google.com/)

1. Transoceanic hop: None
   This traceroute stays within the US, no transoceanic hops.
2. RTT of last hop is 4.414ms, thus max distance of server is:
   Speed * Time = 2*10^8 * 0.004414 = 88280 meters = 88.28 km
3. Reason for lower RTT in subsequent hops:
   Routing change, more optimized path taken hence shorter propagation distance.

### Traceroute to [www.auckland.ac.nz](http://www.auckland.ac.nz/)

1. Transoceanic hop: From hop 10 onward it's transpacific.
2. RTT of last hop is 206.567ms, thus max distance of server is: Speed * Time = 2*10^8 * 0.206567 = 41313400 meters = 41331 km
3. Same as above, lower RTT in subsequent hops could be due to routing change and path optimization.

## 5 HTTP

![image-20231103224114480](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231103224114480.png)

1. **Using Non-Persistent HTTP:**

   - 10 requests x 2 (request and response) x 40 milliseconds (RTT) = 800 milliseconds
   - Plus image transfer time: 10 images x 10 MB / 500 MB/s = 200 milliseconds

   Total time: 800 milliseconds + 200 milliseconds = 1000 milliseconds

2. **Using Persistent HTTP:**

   Calculation:

   - 1 connection establishment and termination, requiring 2 x 40 milliseconds = 80 milliseconds
   - 10 requests x 40 milliseconds (RTT) = 400 milliseconds
   - Plus image transfer time: 10 images x 10 MB / 500 MB/s = 200 milliseconds

   Total time: 80 milliseconds + 400 milliseconds + 200 milliseconds = 680 milliseconds

3. **Using Pipelined Persistent HTTP:**

   Calculation:

   - 1 connection establishment and termination, requiring 2 x 40 milliseconds = 80 milliseconds
   - 10 requests x 40 milliseconds (RTT) = 400 milliseconds
   - Image transfer time: 10 images x 10 MB / 500 MB/s = 200 milliseconds

   Total time: 80 milliseconds + 400 milliseconds + 200 milliseconds = 680 milliseconds

## 6 Client-Server

### Imagine spreading an F-bit file among N peers using a client-server structure. Let the server have a maximum upload capacity μs, and each client c has a download capacity dc. Assume the server can serve multiple clients simultaneously and fluidly set the rate for each client rc.

#### 1. Suppose μs/N ≤ dmin, where dmin = minc dc is the minimum download rate. How would you set the rates rc for each client so that the file is fully distributed to all clients in a minimum time? (i.e. you are minimizing the time that the slowest client receives the file.) What would the distribution time be?

#### 2. Suppose now that μs/N > dmin. How would you set the rates rc now to fully distribute the file to the clients in a minimum time? And what would this time be?

**Case 1: μs/N ≤ dmin**

In this scenario, the server's maximum upload capacity is limited by the minimum download capacity of the clients. To minimize the time it takes to distribute the file to all clients, we evenly distribute the server's upload capacity among the clients:

1. Set the rate rc for each client to dmin. This ensures that each client receives data at their minimum download rate, so no client has to wait for an extended period.
2. The total size of the file is F bits, and the server's maximum upload capacity is μs. Therefore, the distribution time is F / μs. Since μs/N ≤ dmin, this time represents the time required for the slowest client to receive the file.

**Distribution time:** F / μs

**Case 2: μs/N > dmin**

In this case, the server's maximum upload capacity allows a higher total upload rate without being limited by the minimum download rate of the clients. Therefore, we can distribute the file more quickly:

1. Calculate the total available upload rate, which is μs.
2. Distribute the server's upload rate evenly among the N clients, with each client's rate rc = μs / N.
3. The total size of the file is F bits, and each client receives data at a rate of rc. Therefore, the distribution time is F / (μs / N) = (F * N) / μs.

In this scenario, the distribution time is shorter because the server's upload capacity allows a faster distribution rate.

**Distribution time:** (F * N) / μs

# 7 DNS

![image-20231103230058770](C:\Users\Jiadong Hong\AppData\Roaming\Typora\typora-user-images\image-20231103230058770.png)

### (1)

```log
; <<>> DiG 9.18.12-0ubuntu0.22.04.3-Ubuntu <<>> @h.root-servers.net www.eecs.mit.edu
; (2 servers found)
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 30289
;; flags: qr rd; QUERY: 1, ANSWER: 0, AUTHORITY: 13, ADDITIONAL: 27
;; WARNING: recursion requested but not available

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 1232
; COOKIE: dab1e4c207fe06bcdfe58d2865450a6147c3b9124937628b (good)
;; QUESTION SECTION:
;www.eecs.mit.edu.              IN      A

;; AUTHORITY SECTION:
edu.                    172800  IN      NS      c.edu-servers.net.
edu.                    172800  IN      NS      g.edu-servers.net.
edu.                    172800  IN      NS      a.edu-servers.net.
edu.                    172800  IN      NS      d.edu-servers.net.
edu.                    172800  IN      NS      k.edu-servers.net.
edu.                    172800  IN      NS      b.edu-servers.net.
edu.                    172800  IN      NS      i.edu-servers.net.
edu.                    172800  IN      NS      h.edu-servers.net.
edu.                    172800  IN      NS      j.edu-servers.net.
edu.                    172800  IN      NS      e.edu-servers.net.
edu.                    172800  IN      NS      f.edu-servers.net.
edu.                    172800  IN      NS      m.edu-servers.net.
edu.                    172800  IN      NS      l.edu-servers.net.

;; ADDITIONAL SECTION:
a.edu-servers.net.      172800  IN      A       192.5.6.30
b.edu-servers.net.      172800  IN      A       192.33.14.30
c.edu-servers.net.      172800  IN      A       192.26.92.30
d.edu-servers.net.      172800  IN      A       192.31.80.30
e.edu-servers.net.      172800  IN      A       192.12.94.30
f.edu-servers.net.      172800  IN      A       192.35.51.30
g.edu-servers.net.      172800  IN      A       192.42.93.30
h.edu-servers.net.      172800  IN      A       192.54.112.30
i.edu-servers.net.      172800  IN      A       192.43.172.30
j.edu-servers.net.      172800  IN      A       192.48.79.30
k.edu-servers.net.      172800  IN      A       192.52.178.30
l.edu-servers.net.      172800  IN      A       192.41.162.30
m.edu-servers.net.      172800  IN      A       192.55.83.30
a.edu-servers.net.      172800  IN      AAAA    2001:503:a83e::2:30
b.edu-servers.net.      172800  IN      AAAA    2001:503:231d::2:30
c.edu-servers.net.      172800  IN      AAAA    2001:503:83eb::30
d.edu-servers.net.      172800  IN      AAAA    2001:500:856e::30
e.edu-servers.net.      172800  IN      AAAA    2001:502:1ca1::30
f.edu-servers.net.      172800  IN      AAAA    2001:503:d414::30
g.edu-servers.net.      172800  IN      AAAA    2001:503:eea3::30
h.edu-servers.net.      172800  IN      AAAA    2001:502:8cc::30
i.edu-servers.net.      172800  IN      AAAA    2001:503:39c1::30
j.edu-servers.net.      172800  IN      AAAA    2001:502:7094::30
k.edu-servers.net.      172800  IN      AAAA    2001:503:d2d::30
l.edu-servers.net.      172800  IN      AAAA    2001:500:d937::30
m.edu-servers.net.      172800  IN      AAAA    2001:501:b1f9::30

;; Query time: 39 msec
;; SERVER: 198.97.190.53#53(h.root-servers.net) (UDP)
;; WHEN: Fri Nov 03 22:57:54 CST 2023
;; MSG SIZE  rcvd: 868
```

- (Server Domain Name): h.root-servers.net
-  (Server IP Address): 198.97.190.53
- there is no answer section so the TTL is unknown.

### (2)

The DNS protocol primarily uses UDP for its query and response mechanism because, compared to TCP, UDP is faster and requires less overhead. UDP is a connectionless protocol, making it more suitable for simple, lightweight, and quick communication. However, if the response size exceeds the maximum UDP payload size or if reliability is crucial, DNS can fall back to using TCP. UDP is the default choice due to its efficiency and low-latency characteristics.