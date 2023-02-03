#!/usr/bin/python3

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

def ImmediateValueDetection(operands,asmList):

    asmList[1] = operands[0].replace('r', '')
    if (len(operands) == 2 ):
        asmList[3] = operands[0].replace('r', '')
        if (operands[1].isdigit()):
            asmList[0] = operands[1]
            asmList[2] = operands[1]
            asmList[5] = 1
            
        else:
            asmList[0] = 0
            asmList[2] = operands[1].replace('r', '')
            asmList[5] = 0
            
    else: #len operands = 3
        if (operands[1].isdigit() and operands[2].isdigit()):
            raise Exception("Illegal Expression : Only 1 Immediate Value Allowed")
        elif (operands[1].isdigit() or operands[2].isdigit()):
                asmList[5] = 1
                if (operands[1].isdigit()):
                     asmList[0] = operands[1]
                     asmList[2] = operands[2].replace('r','')
                     asmList[3] = operands[1]
                else:
                    asmList[0] = operands[2]
                    asmList[2] = operands[2]
                    asmList[3] = operands[1].replace('r','')
        else:
            asmList[0] = 0
            asmList[2] = operands[2].replace('r', '')
            asmList[3] = operands[1].replace('r', '')
            asmList[5] = 0  
    return asmList

def ParseBCC(line):
    bcc = ["B", "BEQ", "BNE", "BLE", "BGE", "BL", "BG"]
    opcode = line.split(" ")[0]
    if (opcode in bcc):
        print("test")
        return 0
    else:
        return opcode

def ReadCode():

    asmList = [""] * 7
    f= open("code.txt", "rt")
    for line in f:
        print(line)
        asmList[4] = ParseBCC(line) #opcode done
        if (asmList[4] == 0):
            opcode = line.split(" ")[0]
            offset = line.split(" ")[1].replace('r', '').replace(',', '')
            asmList = [offset, 0, 0, 0, 0, 1, 0, opcode]
        else:
            operands = line.split(" ")[1:]
            operands = DecodeOperand(operands)
            asmList = ImmediateValueDetection(operands,asmList)
            asmList[6] = 0
            
            
        print(asmList)
    f.close()
    return 0

def ReadImmédiateValue():
    return 0

def ReadDestinationRegister():
    return 0

def ReadSecondOperand():
    return 0

def ReadFirstOperand():
    return 0

def ReadOperationCode():
    return 0

def ReadImmédiateValueFlag():
    return 0

ReadAlways0 = "000"

def ReadBranchConditionCode():
    return 0

def ReadAll():
    return ReadImmédiateValue() + ReadDestinationRegister() + ReadSecondOperand() + ReadFirstOperand() + ReadOperationCode() + ReadImmédiateValueFlag() + ReadAlways0 + ReadBranchConditionCode() 

ReadCode()