####### Objectif #######

Lire des instructions asm et les transformer sur 32 bits (en binéaire):

exemple : mov eax,3


0-7   Immediate value (immediate value = 3)
8-11  Destination register (eax)
12-15 Second operand (3)
16-19 First operand (eax)
20-23 Operation code (voir 2.5.4)
24    Immediate value flag (1 = oui)
25-27 always set to 0 (=0 pour remplir les 32 bits)
28-31 Branch condition code (voir 2.5.2)


exemple : mov eax,ebx


0-7   Immediate value (immediate value = rien)
8-11  Destination register (eax)
12-15 Second operand (ebx)
16-19 First operand (eax)
20-23 Operation code (voir 2.5.4)
24    Immediate value flag (0 = non)
25-27 always set to 0 (=0 pour remplir les 32 bits)
28-31 Branch condition code (voir 2.5.2)