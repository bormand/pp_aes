print("#ifndef SBOX_GEN_H")
print("#define SBOX_GEN_H")
print("")

def gfmul3(a):
    if a & 0x80:
        return (a << 1) ^ 0x11b ^ a
    return (a << 1) ^ a

exp_t = {}
log_t = {}

a = 1
exp_t[0] = 1
log_t[0] = 0
for i in range(1, 256):
    a = gfmul3(a)
    exp_t[i] = a
    log_t[a] = i

def gfinv(a):
    if a == 0:
        return 0
    return exp_t[255-log_t[a]]

def affine(a):
    s = 0
    for i in range(0, 5):
        s ^= a
        a = (a << 1 | a >> 7) & 0xFF
    return s

for i in range(0, 256):
    print("#define BPP_AES_SBOX_N_0x{0:02x} 0x{1:02x}".format(i, affine(gfinv(i)) ^ 0x63))

print("")
print("#endif")