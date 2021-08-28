""" Generate a microstep table """
import math
microsteps = 100//5# Since speed is ~1-100, then the microstep count should be a divisor of 100

table_values = [
    int(round(0x0FFF * math.sin(math.pi / (2 * microsteps) * i)))
    for i in range(microsteps + 1)
]
table_values = [str(val) for val in table_values]
print(f"static const U16 MICRO_STEP_TABLE[{ len(table_values) }] = {{ { ', '.join(table_values) } }};")
