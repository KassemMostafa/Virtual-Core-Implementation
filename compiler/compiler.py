#!/usr/bin/python3
import struct
import os
import re

#TODO => detect hex and then convert to int
# for this => only ope1 and ope2 can be hex, so take esmElement[2 et 3] and see if they're hex

def DecodeOperand(operands):
    
    if (len(operands) == 2):
        operands[0] = operands[0].replace(',', '')
        operands[1] = operands[1].replace('\n', '')
    elif (len(operands) == 3):
        operands[0] = operands[0].replace(',', '')
        operands[1] = operands[1].replace(',', '')
        operands[2] = operands[2].replace('\n', '')
    else:
        raise Exception("Illegal Expression : Max 2 operands Allowed")
    print(operands)
    
    return operands




def ImmediateValueDetection(operands,asmListElement):

    asmListElement[1] = operands[0].replace('r', '')
    if (len(operands) == 2 ):
        asmListElement[3] = operands[0].replace('r', '')
        if (operands[1].isdigit()):
            asmListElement[0] = operands[1]
            asmListElement[2] = operands[1]
            asmListElement[5] = 1
            
        else:
            asmListElement[0] = 0
            asmListElement[2] = operands[1].replace('r', '')
            asmListElement[5] = 0
            
    else: #len operands = 3
        if (operands[1].isdigit() and operands[2].isdigit()):
            raise Exception("Illegal Expression : Only 1 Immediate Value Allowed")
        elif (operands[1].isdigit() or operands[2].isdigit()):
                asmListElement[5] = 1
                if (operands[1].isdigit()):
                     asmListElement[0] = operands[1]
                     asmListElement[2] = operands[2].replace('r','')
                     asmListElement[3] = operands[1]
                else:
                    asmListElement[0] = operands[2]
                    asmListElement[2] = operands[2]
                    asmListElement[3] = operands[1].replace('r','')
        else:
            asmListElement[0] = 0
            asmListElement[2] = operands[2].replace('r', '')
            asmListElement[3] = operands[1].replace('r', '')
            asmListElement[5] = 0  
    return asmListElement

def ParseBCC(line): #Detects if there's a BCC or not, if it does, returns 0 (as bcc is in a different area) else returns opcode to opcode case asmListElement[4]
    bcc = ["B", "BEQ", "BNE", "BLE", "BGE", "BL", "BG"]
    opcode = line.split(" ")[0]
    if (opcode in bcc):
        print("test")
        return 0
    else:
        return opcode

def CheckHex(hex):
    hex_regex = r"^(0x|0X)[0-9a-fA-F]{1,16}$"
    match = re.search(hex_regex, hex)
    if match:
        print("Valid\n")
        return int(hex,0)
    else:
        print("unvalid\n")
        if hex.isdigit():
            return int(hex)
        else:
            raise Exception("Erreur de Compilation : Invalid IV")

def ReadCode(): # if no BCC returns [IV, dest, ope2, ope1, opcode, IV Flag, 0 (cause no BCC)], if BCC returns [offset, 0,0,0,0,1,BCC]
#limitation : IV is written as ope1 or ope2, need to compare ope2 and ope1 value to IV for operation with 
    
    f= open("compiler/codeTest.txt", "rt")
    asmList= []
    for line in f:
        asmListElement = [""] * 7
        print(line)
        asmListElement[4] = ParseBCC(line) 
        if (asmListElement[4] == 0):
            opcode = line.split(" ")[0]
            offset = line.split(" ")[1].replace('r', '').replace(',', '')
            asmListElement = [offset, 0, 0, 0, 0, 1, opcode]
        else:
            operands = line.split(" ")[1:]
            operands = DecodeOperand(operands)
            asmListElement = ImmediateValueDetection(operands,asmListElement)
            asmListElement[6] = 0
            
        asmListElement[0] = int(asmListElement[0])
        asmListElement[1] = int(asmListElement[1])
        asmListElement[2] = CheckHex(asmListElement[2])
        asmListElement[3] = CheckHex(asmListElement[3])
        asmListElement[5] = int(asmListElement[5])
        print("Before append")
        print(asmList)
        print("After Append")
        asmList.append(asmListElement)
        print(asmList)
        print('here')
    f.close()
    return asmList

# asmList = [0,0,0,0,"ANDa",0,"BEQz"]























##LOICK
def ReadImmédiateValue(asmList0, asmList5):
    ret = "00000000"
    if asmList5 :
        ret = str(bin(asmList0))[2:]
        ret = "00000000" + ret
        a = len(ret)-8
        ret = ret[a:]
    print("1 : " + ret)
    ret2=""
    for i in range(2):
        ret2=ret2 +str(ret[4 - i * 4:8- i * 4])
    ret = ret2
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
    if asmList5 == 0 :
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

def ReadBCC(asmList1):
    if asmList1 < 0:
        ret = str(bin(asmList1))[3:]
        print( "ret21 = " + ret)
        ret = "0" * 27 +ret
        a = len(ret) - 27
        print( "ret2 = " + ret)

        ret = ret[a:]
        print( "ret2 = " + ret)
        print("amslist1 : " + str(asmList1))
        ret = "1" + ret


    else:
        ret = str(bin(asmList1))[2:]
        print("ret21 = " + ret)
        ret = "0" * 27 + ret
        a = len(ret) - 27
        print("ret2 = " + ret)

        ret = ret[a:]
        print("ret2 = " + ret)
        print("amslist1 : " + str(asmList1))
        ret = "0"+ret

    print("2 : " + ret)
    return ret

def ReadOneLine(asmListElement,binary_file): #flag true on first call, false otherwise
    if asmListElement[6] == 0 :
        ret = ReadImmédiateValue(asmListElement[0], asmListElement[5]) + ReadDestinationRegister(asmListElement[1]) + ReadSecondOperand(asmListElement[2],asmListElement[5]) + ReadFirstOperand(asmListElement[3]) + ReadOperationCode(asmListElement[4]) + ReadImmédiateValueFlag(asmListElement[5]) + ReadAlways0 + ReadBranchConditionCode(asmListElement[6])
        ret2 = ""
        for i in range(8):
            ret2 = ret2 + str(ret[28 - i * 4:32 - i * 4])
        print("ret after bin : " + ret2)
        ret = int(ret2, 2)
    else :
        ret = ReadBranchConditionCode(asmListElement[6])+ReadBCC(asmListElement[0])
        print("ret after bin : " + ret)
        ret = int(ret, 2)
    print("ret bfore bin" + str(ret))
    
    print(ret)
    binary_file.write(struct.pack('>I',ret))
    print("written to file")

    return ret

def ReadAll(asmList):
    with open("binary.bin","wb") as binary_file:
        for asmElement in asmList:
            print("rol")
            ReadOneLine(asmElement, binary_file)
            print("asmElement is")
            print(asmElement)
            print("hello")
    binary_file.close()
    return 0
asmList = ReadCode()
ReadAll(asmList)

