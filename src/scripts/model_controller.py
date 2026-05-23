# import tensorflow as tf
# #from tensorflow.keras import layers
# #from tensorflow.keras import Model
# from keras import layers
# from keras import Model


# def build_policy_network(state_dim: int,action_dim: int):

#     # INPUT

#     state_input = layers.Input(
#         shape=(state_dim,),
#         name="state"
#     )

#     # HIDDEN LAYERS


#     x = layers.Dense(256,activation="relu")(state_input)
#     x = layers.Dense(256,activation="relu")(x)
#     x = layers.Dense(128,activation="relu")(x)

#     # OUTPUT

#     torque_output = layers.Dense(action_dim, activation= "tanh",name= "torques")(x)

#     # MODEL
    
#     model = Model(inputs=state_input, outputs = torque_output, name="walking_policy")

#     return model


if __name__ == "__main__":
    import sys
    from time import time,ctime
    
    print('Today is', ctime(time()))
    print('executable:', sys.executable)
    print('version', sys.version)

