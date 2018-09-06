package go_network_test

import (
	"log"
	"net"
	"testing"
)

const client_port = "localhost:8501"
const server_port = "localhost:8500"

var saddr,caddr *net.UDPAddr
var ccon *net.UDPConn
var client_buffer = make([]byte, 1024)
var setupDone bool

func setUp() {
	var err error
	saddr, err = net.ResolveUDPAddr("udp", server_port)
	if err != nil {
		panic(err)
	}
	caddr, err = net.ResolveUDPAddr("udp", client_port)
	if err != nil {
		panic(err)
	}
	ccon, err = net.ListenUDP("udp", caddr)
	if err!=nil {
		panic(err)
	}
}

func server() {

	l, err := net.ListenUDP("udp", saddr)
	if err != nil {
		panic(err)
	}

	in := make([]byte, 1024)

	for {
		_, _, err := l.ReadFromUDP(in)
		if err != nil {
			log.Fatal("ReadFromUDP failed:", err)
		}
		l.WriteToUDP(in,caddr)
	}
}

func BenchmarkMain(b *testing.B){
	if setupDone {
		return
	}
	setUp()
	go server()
	setupDone=true
}

func BenchmarkNetworkPingPong(b *testing.B) {
	for i:=0; i<b.N; i++ {
		ccon.WriteTo(client_buffer,saddr)
		ccon.ReadFromUDP(client_buffer)
	}
}
