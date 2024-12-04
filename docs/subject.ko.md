
# IRC

## [Chapter 1] Introduction

**internet Relay Chat**혹은 IRC는 인터넷 텍스트 채팅 프로토콜이다.
이 프로토콜은 공개, 비공개 실시간 메시지를 제공한다. 사용자는 메시지를 교환하고, 채널에 들어갈 수 있다.

IRC 클라이언트는 채널에 들어가기 위해 IRC서버에 연결한다. IRC서버는 서로 연결되어 네트워크를 형성한다.

## [Chapter 2] General Rules

- 프로그램이 어떤 상황에서도(메모리가 부족한 경우에도) 충돌하지 않아야 하며, 예기치 않게 종료되어서는 안 됩니다. 이 경우 프로젝트가 작동하지 않는 것으로 간주되며 등급은 0점이 됩니다.
- 소스 파일을 컴파일할 Makefile을 제출해야 합니다. 다시 연결해서는 안 됩니다.
- 메이크파일에는 적어도 규칙($NAME)이 포함되어 있어야 합니다: 
	`all, clean, fclean, re`

- c++ 및 플래그 `-Wall -Wextra -Werror`로 코드를 컴파일합니다
- 코드는 `C++ 98` 표준을 준수해야 합니다. 그런 다음 플래그 `-std=c++98`을 추가해도 컴파일됩니다.
- 항상 가능한 한 가장 많은 C++ 기능을 사용하여 개발하세요(예: `<string.h>` 대신 `<cstring>`을 선택합니다). C 함수를 사용할 수 있지만 가능하면 항상 C++ 버전을 선호합니다.
- 외부 라이브러리 및 부스트 라이브러리는 금지되어 있습니다.

## [Chapter 3] Mandatory part
|프로그램명|irceserv|
|:--|:--|
|포함파일|`Makefile`, `*.{h,hpp}`, `*.cpp`, `*.tpp`, `*.ipp`</br> an optional configuration file |
|Makefile|NAME, all, clean, fclean, re|
|허용 외부함수|Everything in c++ 98.</br>socket, close, setsockopt, getsockname,getprotobyname, gethostbyname, getaddrinfo,freeaddrinfo, bind, connect, listen, accept, htons, htonl, ntohs, ntohl, inet_addr, inet_ntoa, send, recv, signal,sigaction, lseek, fstat, fcntl, poll(or equivalent)|
|설명|C++ 98로 만든 IRC server|

C++로 IRC서버를 만들어야합니다. 

- 클라이언트를 개발해서는 안 됩니다.
- 서버 간 통신을 처리해서는 안 됩니다.

실행 파일은 다음과 같이 실행됩니다:
./ircserv \<port\> \<password\>

- port: IRC 서버가 수신을 위해 수신을 수신할 포트 번호
IRC 연결.
- password: 연결 비밀번호. 서버에 연결하려는 모든 IRC 클라이언트가 이 비밀번호를 사용해야 합니다.

> 서브젝트와 평가에서 poll()이 언급되지만, 이와 동등한 `select()`, `kqueue()`, `epoll()`을 사용할 수 있습니다.


### 요구사항

- 서버는 반드시 여러명의 유저를 동시에 처리할 수 있어야하며 절대 끊어서는 안된다.
- Forking은 허락되지 않습니다. 모든 I/O 작업은 non-blocking으로 처리해야합니다.
- 모든 작업(read, write, listen 등등)은 반드시 **하나의 `poll()`(혹은 select, kqueue,epoll)** 을 통해서 처리해야합니다. 

> ⚠️ non-blocking으로 file descripter를 다뤄야하기 때문에 `poll`을 사용하지 않고 read/recv 또는 write/send 할수 있고 서버는 blocking되지 않습니다. 
> 하지만 이 경우 더 많은 시스템 리소스를 소비하게 됩니다.
> 따라서 `poll`을 사용하지 않고 file descripter를 read/recv 또는 write/send하면 0점을 받게 됩니다.

- 몇몇 IRC클라이언트가 존재합니다. 그중 하나를 **reference**로 채택하세요. 이 레퍼런스 클라이언트를 사용해서 평가를 진행하게 됩니다.

- 선택한 레퍼런스 클라이언트는 반드시 에러없이 서버에 연결할 수 있어야합니다. 
- 클라이언트와 서버사이에 통신은 TCP/IP(v4 or v6)를 통해서 연결합니다. 
- 레퍼런스 클라이언트와 구현한 서버는 반드시 공식적인 다른 IRC서버와 비슷해야합니다. 하지만, 아래의 기능들만 구현하면 됩니다. 

### 구현 기능목록 
- 인증(authenticate)
- 닉네임, 유저네임 설정
- 채널 접속
- 클라이언트를 이용하여 private메시지 송수신
- 클라이언트로부터 채널에 보내진 모든 메시지는 채널에 들어와있는 모든 유저에게 반드시 전달되어야합니다. 
- 운영자와 일반 사용자가 있어야합니다.
- 채널운영자는 다음과 같은 명령어를 사용할 수 있도록 구현해야합니다. 
	- KICK
	- INVITE
	- TOPIC
	- MODE
		- i
		- t
		- k
		- o
		- l
- 코드를 깔끔하게 작성하길 기대합니다.

##  Mac OS환경을 위해서
- MacOS는 다른 Unix OS들과는 다른 방식으로 write를 구현했습니다. 그래서 fcntl을 사용하느걸 허용합니다. non-blocking모드에서 file Descripter를 사용해 다른 UnixOS들과 유사하게 동작하도록 하세요.

- 하지만 fcntl의 다형성 중 허용되는건 오직 `fcntl(fd, f_setFL, O_NONBLOCK)`뿐입니다. 다른건 모두 금지됩니다.

## 테스트 예시

가능한 모든 에러와 이슈들에 대해서 검증하세요(부분 데이터 수신, 낮은 대역폭 등등).

서버에 보내진 메세지를 서버가 정상적으로 처리하는지 확신하기 위해서 nc를 사용하여 아래와 같이 간단히 테스트할 수 있습니다. 

```bash
\$> nc -c 127.0.0.1 6667
com^Dman^Dd
\$>
```

ctrl+D를 사용하여 몇가지 부분의 커맨드를 보내세요: 'com', 그리고 'man', 'd\n'.
커맨드를 처리하기 위해서, 우선 받은 패킷들을 집계하여 재구성해야합니다. 

## [Bonus part]

실제 IRC 서버처럼 보이도록 IRC 서버에 추가할 수 있는 추가 기능은 다음과 같습니다.

- handle file transfer
- bot

> 보너스 부분은 필수 부분이 완벽한 경우에만 평가됩니다. 완벽하다는 것은 필수 부분이 통합적으로 이루어졌으며 오작동 없이 작동한다는 것을 의미합니다. 필수 요건을 모두 통과하지 못한 경우 보너스 부분은 전혀 평가되지 않습니다.

