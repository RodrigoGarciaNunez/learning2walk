




import sys
import time
import random as rd
#import scripts.model_controller


def return_output(output: list):
    output = [rd.uniform(-50, 50) for _ in range(len(output))]
    return output

if __name__ == "__main__":
    
    while(True):
        print(input_mj.joint_pos)

        output_mj.torque = return_output(output_mj.torque)

        print(output_mj.torque)
        
        # print("hola")
        # 
        # print(type(output_mj.torque))
        time.sleep(2)


