Import("env")

# Access to global construction environment
build_tag = env['PIOENV']

# Dump construction environment (for debug purpose)
# print(env.Dump())

# Rename binary according to environnement/board
# ex: firmware_mainmcu_esp32dev.bin or firmware_mainmcu_nodemcuv2.bin
env.Replace(PROGNAME="firmware_mainmcu_%s" % build_tag)