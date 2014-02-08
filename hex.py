print("#ifndef HEX_GEN_H")
print("#define HEX_GEN_H")
print("")

print("\n".join([ "#define BPP_UNHEX_N_0x{0:02x} ({1})".format(x,
    ")(".join([c for c in "{0:08b}".format(x)])) for x in range(0, 256)]))

print("\n".join([ "#define BPP_HEX_N_{1:08b} 0x{0:02x}".format(x, x) for x in range(0, 256)]))

print("")
print("#endif")