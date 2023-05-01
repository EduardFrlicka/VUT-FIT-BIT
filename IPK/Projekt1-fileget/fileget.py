#!/usr/bin/env Python3

import argparse
from socket import *
import sys
import select
import re
from os import makedirs,path,remove,rmdir


def main():
    argParser = argparse.ArgumentParser()
    argParser.add_argument("-n",required=True)
    argParser.add_argument("-f",required=True)
    args = argParser.parse_args()

    re_result=re.search(r"^(\d+.\d+.\d+.\d+):(\d+)$",args.n)
    if not re_result:
        print("Wrong Ip address")
        sys.exit(1)
    
    name_addr = (re_result.group(1),int(re_result.group(2)))
    file_arg=args.f

    re_result=re.search(r"^fsp://((\w*\.)*\w*)/((\w+/)*)?(\w*(\.\w+)?)$",file_arg)
    if  re_result:
        server_name = re_result.group(1)
        file_path=re_result.group(3)
        file_name=re_result.group(5)

        file_addr = get_file_addr_from_url(name_addr,server_name)
        get_file_from_url(file_addr,file_name,file_path,server_name)
        print("OK")
        sys.exit(0)

    re_result=re.search(r"^fsp://((\w*\.)*\w*)/\*$",file_arg)
    if re_result:
        server_name = re_result.group(1)
        get_all_server(name_addr,server_name)
        print("OK")
        sys.exit(0)
    
    re_result=re.search(r"^fsp://((\w*\.)*\w*)/((\w+/)+)\*$",file_arg)
    if re_result:
        server_name = re_result.group(1)
        file_path = re_result.group(3)
        get_all_dir(name_addr,server_name,file_path)
        print("OK")
        sys.exit(0)

    print("reg err main")
    sys.exit(2)


def remove_index():
    try:
        remove("tmp/index")
    except: 
        print("cannot del tmp/index")
        sys.exit(3)

    try:
        rmdir("tmp")
    except: 
        print("cannot del tmp/")
        sys.exit(4)


def get_all_dir(name_addr:tuple,server_name:str, dir):
    file_addr = get_file_addr_from_url(name_addr,server_name)
    get_file_from_url(file_addr,"index","",server_name,"tmp/")

    try:
        with open("tmp/index","r") as index_file:
            for line in index_file:
                file_path,file_name = path.split(line.strip("\r\n"))
                file_path+="/"
                if(dir==file_path):
                    get_file_from_url(file_addr,file_name,file_path,server_name)
    except:
        print("cannot open tmp/index")
        sys.exit(5)

    remove_index()
    return 0


def get_all_server(name_addr,server_name):
    file_addr = get_file_addr_from_url(name_addr,server_name)
    get_file_from_url(file_addr,"index","",server_name,"tmp/")

    try:
        with open("tmp/index","r") as index_file:
            for line in index_file:
                file_path,file_name = path.split(line.strip("\r\n"))
                file_path+="/"
                get_file_from_url(file_addr,file_name,file_path,server_name,file_path)
    except Exception as e:
        print(e)
        print("cannot open tmp/index")
        sys.exit(6)
    remove_index()
    return 0


def get_file_addr_from_url(name_addr:tuple,server_name:str,buf:int=1024):
    name_s = socket(AF_INET,SOCK_DGRAM)
    name_s.settimeout(30)
    name_request = f"WHEREIS {server_name}\r\n\r\n"
    name_request = bytes(name_request,"utf-8")
    
    try:
        name_s.sendto(name_request,name_addr)
    except:
        print("send err")
        name_s.close()
        sys.exit(8)
    
    try:
        response=name_s.recv(buf)
    except:
        print("Recv err, header")
        name_s.close()
        sys.exit(9)

    name_s.close()

    if b"ERR" in response:
        print("Server returned Err")
        sys.exit(10)
    
    re_result = re.search(rb"OK (\d+.\d+.\d+.\d+):(\d+)",response)
    if not re_result:
        print("Reg err")
        sys.exit(11)
    
    return (re_result.group(1),int(re_result.group(2)))


def get_file_from_url(file_addr:tuple, file_name:str,file_path:str,server_name:str,target_path:str="",buf:int=1024):
    file_s=socket(AF_INET,SOCK_STREAM)
    file_s.settimeout(30)
    try:
        file_s.connect(file_addr)
    except:
        print("connect failed")
        sys.exit(12)
    
    request = f"GET {file_path+file_name} FSP/1.0\r\nHostname: {server_name}\r\nAgent: xfrlic00\r\n\r\n"
    request=bytes(request,"UTF-8")
    
    try:
        file_s.send(request)
    except:
        print("send failed")
        file_s.close()
        sys.exit(13)
    
    try:
        response=file_s.recv(buf)
    except:
        print("Recv err, header")
        file_s.close()
        sys.exit(14)
    
    if not b"\r\n\r\n" in response:
        file_s.close()
        print("Bad response")
        sys.exit(15)
    
    pos = response.find(b"\r\n\r\n")+4
    header = response[:pos]
    
    if not b"Success" in header:
        file_s.close()
        print("server err")
        sys.exit(16)
    
    re_result = re.search(rb"Length:(\d+)",header)
    file_size=int(re_result.group(1))
    save = response[pos:]
    real_size = len(save)

    if(target_path!=""):
        try:
            makedirs(target_path,exist_ok=True)
        except:
            file_s.close()
            print("makedir failed")
            sys.exit(17)
    
    try:
        file = open(target_path + file_name,"wb")
    except:
        file_s.close()
        print("failed to open file:",file_name)
        sys.exit(18)
    
    try:
        file.write(save)
    except:
        file_s.close()
        print("file write err")
        sys.exit(19)

    while real_size<file_size:
        try:
            response=file_s.recv(buf)
        except Exception as e:
            file_s.close()
            print(e)
            print("Recv err",real_size)
            sys.exit(20)
    
        file.write(response)
        real_size+=len(response)

    return 0


if __name__=="__main__":
    main()
