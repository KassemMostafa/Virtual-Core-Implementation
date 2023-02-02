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