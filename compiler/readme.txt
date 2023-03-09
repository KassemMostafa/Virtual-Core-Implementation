####### Objectif #######

Lire des instructions asm et les transformer sur 32 bits (ouvrir le fichier en mode binéaire wb):

exemple : mov eax,3 

0-7   Immediate value (immediate value = 3) (asmList[0])
8-11  Destination register (eax) (asmList[1])
12-15 Second operand (3) (asmList[2])
16-19 First operand (eax) (asmList[3])
20-23 Operation code (voir 2.5.4) (asmList[4])
24    Immediate value flag (1 = oui) (asmList[5])
25-27 always set to 0 (=0 pour remplir les 32 bits)
28-31 Branch condition code (voir 2.5.2) (asmList[6])

exemple : add eax, ebx, ecx

0-7   Immediate value (immediate value = 0) (asmList[0])
8-11  Destination register (eax) (asmList[1])
12-15 Second operand (ecx) (asmList[2])
16-19 First operand (ebx) (asmList[3])
20-23 Operation code (voir 2.5.4) (asmList[4])
24    Immediate value flag (0 = non) (asmList[5])
25-27 always set to 0 (=0 pour remplir les 32 bits)
28-31 Branch condition code (voir 2.5.2) (asmList[6])
 
exemple : add eax, ebx, 6

0-7   Immediate value (immediate value = 6) (asmList[0])
8-11  Destination register (eax) (asmList[1])
12-15 Second operand (6) (asmList[2])
16-19 First operand (ebx) (asmList[3])
20-23 Operation code (voir 2.5.4) (asmList[4])
24    Immediate value flag (1 = oui) (asmList[5])
25-27 always set to 0 (=0 pour remplir les 32 bits)
28-31 Branch condition code (voir 2.5.2) (asmList[6])

exemple : mov eax,ebx


0-7   Immediate value (immediate value = rien)
8-11  Destination register (eax)
12-15 Second operand (ebx)
16-19 First operand (eax)
20-23 Operation code (voir 2.5.4)
24    Immediate value flag (0 = non)
25-27 always set to 0 (=0 pour remplir les 32 bits)
28-31 Branch condition code (voir 2.5.2)

exemple : BEQ 25 // : CMP eax, ebx => set flag selon comparaison

0-7   Immediate value (immediate value = 25)
8-11  Destination register (0)
12-15 Second operand (0)
16-19 First operand (0)
20-23 Operation code (0)
24    Immediate value flag (1 = oui)
25-27 always set to 0 (=0 pour remplir les 32 bits)
28-31 Branch condition code (BEQ)

