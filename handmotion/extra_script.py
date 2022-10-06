Import("env")

# Access to global construction environment
build_tag = env['PIOENV']

# Dump construction environment (for debug purpose)
# print(env.Dump())

# Rename binary according to environnement/board
# ex: firmware_handmotion_esp32dev.bin or firmware_handmotion_nodemcuv2.bin
env.Replace(PROGNAME="firmware_handmotion_%s" % build_tag)