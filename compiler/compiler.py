#!/usr/bin/python3


asmList = [0,0,0,0,"ANDa",0,"BEQz"]

def ReadImmédiateValue(asmList0, asmList5):
    ret = "00000000"
    if asmList5 :
        ret = str(bin(asmList0))[2:]
        ret = "00000000" + ret
        a = len(ret)-8
        ret = ret[a:]
    print("1 : " + ret)
    return ret

def ReadDestinationRegister(asmList1):
    ret = str(bin(asmList1))[2:]
    ret = "0000" + ret
    a = len(ret)-4
    ret = ret[a:]
    print("2 : " + ret)
    return ret

def ReadSecondOperand(asmList2, asmList5):
    ret = "0000"
    if asmList5:
        ret = str(bin(asmList2))[2:]
    ret = "0000" + ret
    a = len(ret)-4
    ret = ret[a:]
    print("3 : " + ret)
    return ret

def ReadFirstOperand(asmList3):
    ret = str(bin(asmList3))[2:]
    ret = "0000" + ret
    a = len(ret)-4
    ret = ret[a:]
    print("4 : " + ret)
    return ret

def ReadOperationCode(asmList4):
    if asmList4 == "AND" :
        ret = "0000"
    elif asmList4 == "ORR" :
        ret = "0001"
    elif asmList4 == "EOR" :
        ret = "0010"
    elif asmList4 == "ADD" :
        ret = "0011"
    elif asmList4 == "ADC" :
        ret = "0100" 
    elif asmList4 == "CMP" :
        ret = "0101"  
    elif asmList4 == "SUB" :
        ret = "0110"
    elif asmList4 == "SBC" :
        ret = "0111"
    elif asmList4 == "MOV" :
        ret = "1000"
    elif asmList4 == "LSH" :
        ret = "1001"
    elif asmList4 == "RSH" :
        ret = "1010"
    else :
        ret = "0000" 
    print("5 : " + ret) 
    return ret

def ReadImmédiateValueFlag(asmList5):
    print("6 : " + str(asmList5))
    return str(asmList5)

ReadAlways0 = "000"

def ReadBranchConditionCode(asmList6):
    if asmList6 == "B" :
        ret = "1000"
    elif asmList6 == "BEQ" :
        ret = "1001"
    elif asmList6 == "BNE" :
        ret = "1010"
    elif asmList6 == "BLE" :
        ret = "1011"
    elif asmList6 == "BGE" :
        ret = "1100" 
    elif asmList6 == "BL" :
        ret = "1101"  
    elif asmList6 == "BG" :
        ret = "1110"
    else :
        ret = "0000"
    print("7 : " + ret)
    return ret

def ReadAll(asmList):
    ret = ReadImmédiateValue(asmList[0], asmList[5]) + ReadDestinationRegister(asmList[1]) + ReadSecondOperand(asmList[2],asmList[5]) + ReadFirstOperand(asmList[3]) + ReadOperationCode(asmList[4]) + ReadImmédiateValueFlag(asmList[5]) + ReadAlways0 + ReadBranchConditionCode(asmList[6]) 
    ret = int(ret, 2)
    ret = ret.to_bytes(4, 'big')
    with open("binary.bin", "wb") as binary_file:
        binary_file.write(ret)
        binary_file.close()
    print(ret)
    print(len(ret))
    return ret

ReadAll(asmList)