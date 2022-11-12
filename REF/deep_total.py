import numpy as np
import tensorflow as tf
from tensorflow.keras import models
from tensorflow.keras.models import Sequential
from tensorflow.keras.callbacks import EarlyStopping
import matplotlib.pyplot as plt
from sklearn.preprocessing import MinMaxScaler
import math
import rank_measure as rm
import pandas as pd
from tensorflow.keras.layers import Dropout, Dense, Activation
from tensorflow.keras.models import load_model 
from keras import backend as K	

K.tensorflow_backend._get_available_gpus()

train_file = np.genfromtxt("./binary_feature_model_train.txt")
test_file = np.genfromtxt("./binary_feature_model_vali.txt")
today_file = np.genfromtxt("./binary_feature_model_test.txt")
#only indicator feature ex 12 input feature
# cci, rsi, bollinger band, 
print('파일 로딩 완료')
# 사용자 임의 조정값
batch_size = 500  # batch size
epochs = 100
input_size  = 250 #qhfb

train_x = []
train_y_ = []
test_x = []
test_y_ = []
code_arr = []
date_arr = []
today_x = []
today_y = []
t_code_arr = []
t_date_arr = []
scores = []
predict_code = []
predict_date = []
record_result_info = []
day = []

for line in train_file:
    x = line[:input_size]
    y_ = line[input_size]
    train_x.append(x)
    train_y_.append(y_)

for line in test_file:
    x = line[:input_size]
    y_ = line[input_size]
    code = line[input_size+1]
    date = line[input_size+2]
    test_x.append(x)
    test_y_.append(y_)
    code_arr.append(code)
    date_arr.append(date)

for line in today_file:
    x = line[:input_size]
    y_ = line[input_size]
    code = line[input_size+1]
    date = line[input_size+2]
    today_x.append(x)
    today_y.append(y_)
    t_code_arr.append(code)
    t_date_arr.append(date)

train_x = np.array(train_x).reshape(-1, input_size)
test_x = np.array(test_x).reshape(-1, input_size)
today_x = np.array(today_x).reshape(-1, input_size)
train_x = pd.DataFrame(train_x)
test_x = pd.DataFrame(test_x)
today_x = pd.DataFrame(today_x)

#train_x = rm.rank_measuring(train_x, input_size) #이부분 일단 스킵
#test_x = rm.rank_measuring(test_x, input_size)
#today_x = rm.rank_measuring(today_x, input_size)

#data scaling
#train_x = minmax_scale(train_x, feature_range=(-1, 1))
#test_x = minmax_scale(test_x, feature_range=(-1, 1))
#today_x = minmax_scale(today_x, feature_range=(-1, 1))

print('train, test 파일 분류 완료')
print(train_x)
print(test_x)


model = Sequential()
model.add(Dense(input_size, activation='tanh', input_shape=(train_x.shape[1],))) # relu 사용 지양
model.add(Dense(input_size * 20,  activation='tanh')) # relu 사용 지양
model.add(Dense(1, activation='tanh'))
model.compile(optimizer='rmsprop', loss='mse', metrics=['mae', 'acc'])
model.summary()
history = model.fit(train_x, train_y_, epochs=epochs, batch_size=batch_size, verbose=1)
pred = model.predict(test_x)

#예측이랑 실제랑 차이 보는 코드
fig = plt.figure(facecolor='white', figsize=(20, 10))
ax = fig.add_subplot(111)
ax.plot(test_y_, label='True')
ax.plot(pred, label='Prediction')
ax.legend()
plt.show()

#에러 출력 코드
val_mse, val_mae, val_acc = model.evaluate(test_x, test_y_, verbose=0)
plt.title('basic model')
plt.plot(history.history['loss'], linestyle='--')
plt.plot(history.history['mean_absolute_error'])
plt.legend(['loss'], loc='upper left')
plt.legend(['mean_absolute_error'], loc='upper left')
plt.ylabel('loss')
plt.xlabel('EPoch')
plt.legend()
plt.show()

#model.save_model('hivol.h5')

#layer_outputs = [layer.output for layer in model.layers]
#activations_model = models.load_model('hivol.h5')
activations = model.predict(today_x, batch_size, verbose=1)
last_layer_activation = activations[-1]
#print(activations[-1])
scores = []
predict_code = []
predict_date = []
record_result_info = []
day = []
f = open("190212_over90.txt", 'w')

for i, predictions in enumerate(last_layer_activation): 
    if predictions>=0.09: #예측치 ~이상인 것만 출력하라 
        #print(predictions)
        #print(t_code_arr[i])
        record_result_info.append([t_code_arr[i], np.array2string(t_date_arr[i]),np.array2string(predictions), 0.0000]) 

record_result_info = sorted(record_result_info)
print(record_result_info)

for i in range(len(record_result_info)):    
    day.append(record_result_info[i][1])

for i in range(len(day)):
    day_tmp = day[i]
    day_count = 0
    for j in range(len(day)):
        if(day[j] == day_tmp):	        
            if(day_count == 0):
                fund_f = open("./fundsim"+ str(day[j]) +"txt","w")
                day_count += 1
            else :
                fund_f = open("./fundsim"+ str(day[j]) +"txt","a")
            comp_str = str(record_result_info[j][0])
            comp_str = comp_str[:-2]    
            date_str = str(record_result_info[j][1])
            pred_str = str(record_result_info[j][2])
            pred_str = pred_str.replace('[','')
            pred_str = pred_str.replace(']','')
            comp_str = "{:0>6s}".format(comp_str)
            if comp_str == '00000n': 
                pass
            else:
                data = "{:0>6s}".format(comp_str) + ", " + date_str[0:8] + ", " + pred_str + ", " + str(record_result_info[j][3]) + "\n"
                fund_f.write(data)
                fund_f.close()
        else :
            continue

for idx, predictions in enumerate(last_layer_activation): #펀심 파일 만들기 코드
    if predictions >= 0.09:
        scores.append(predictions)
        predict_code.append(t_code_arr[idx])
        predict_date.append(t_date_arr[idx])
        print(predictions)
        print(t_code_arr[idx])
        print(t_date_arr[idx])
        f.write(np.array2string(predictions))
        f.write(' ')
        f.write(np.array2string(t_code_arr[idx]))
        f.write(' ')
        f.write(np.array2string(t_date_arr[idx]))
        f.write('\n')
f.close()

import matplotlib.pyplot as plt

plt.hist(last_layer_activation, 50, normed=True, facecolor='g', alpha=1.0)
plt.show() 
