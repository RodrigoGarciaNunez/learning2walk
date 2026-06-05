

# import tensorflow as tf
# #from tensorflow.keras import layers
# #from tensorflow.keras import Model
# from keras import layers
# from keras import Model




import sys
import time
import random as rd
#import scripts.model_controller


def return_output(output: list):
    output = [rd.uniform(-50, 50) for _ in range(len(output))]
    return output

if __name__ == "__main__":
    
    while(True):
        print(input_mj.left_foot_contact)
        print(input_mj.right_foot_contact)

        output_mj.torque = return_output(output_mj.torque)

        print(output_mj.torque)
        
        # print("hola")
        # 
        # print(type(output_mj.torque))
        time.sleep(2)





# #def build_policy_network(state_dim: int,action_dim: int):

#     state_input = layers.Input(
#         shape=(state_dim,),
#         name="state"
#     )


#     x = layers.Dense(256,activation="relu")(state_input)
#     x = layers.Dense(256,activation="relu")(x)
#     x = layers.Dense(128,activation="relu")(x)


#     torque_output = layers.Dense(action_dim, activation= "tanh",name= "torques")(x)

    
#     model = Model(inputs=state_input, outputs = torque_output, name="walking_policy")

#     return model