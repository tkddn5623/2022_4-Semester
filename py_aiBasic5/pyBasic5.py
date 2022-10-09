import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

class BC(nn.Module):
    def __init__(self):
        super().__init__()
        self.linear = nn.Linear(8, 1)
        self.sigmoid = nn.Sigmoid()
    def forward(self ,x):
        return self.sigmoid(self.linear(x))
    
torch.manual_seed(1)
# Q1) 6개 데이터 샘플을 training 데이터로 입력하시오. 입력값은 (키, 몸무게)(가능한 현실에 있을법한 키(cm)와 몸무게(kg) 값으로 입력하시오.), 출력값은 비만인 경우 (1) 비만이 아닌 경우는 (0)으로 표기할 것.  
x_data =  [[170, 65], [171, 67], [172, 70], [173, 67], [180, 80], [181, 100]]
y_data =  [[1],       [0],       [1],       [1],       [1],       [0]]
x_train = torch.FloatTensor(x_data)
y_train = torch.FloatTensor(y_data)

# 모델 초기화
W = torch.zeros((2, 1), requires_grad=True)
b = torch.zeros(1, requires_grad=True)

# Q2) 학습이 진행되도록 알맞은 Optimizer, 학습률을 변경하여 입력하시오. 왜 학습률을 이렇게 하였는가 주석으로 반드시 설명하시오.
model = BC()
optimizer = optim.SGD(model.parameters(), lr = 0.1)

# Q3) 학습이 잘 진행되도록 알맞게 epoch 수를 조정 하시오. 왜 epoch 수를 이렇게 설정 하였는가 주석으로 반드시 설명하시오.
nb_epochs = 100
for epoch in range(nb_epochs + 1):

    # Cost 계산
    hypothesis = torch.sigmoid(x_train.matmul(W) + b) # or .mm or @
    cost = -(y_train * torch.log(hypothesis) + 
             (1 - y_train) * torch.log(1 - hypothesis)).mean()

    # cost로 H(x) 개선
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    # 100번마다 로그 출력
    if epoch % 100 == 0:
        print('Epoch {:4d}/{} Cost: {:.6f}'.format(
            epoch, nb_epochs, cost.item()
        ))
# Q4) x_test에 적당한 입력(키, 몸무게)를 입력하여 결과를 확인하시오.
x_test = [175, 55]
hypothesis = torch.sigmoid(x_test.matmul(W) + b)
cost = -(y_train * torch.log(hypothesis) + 
             (1 - y_train) * torch.log(1 - hypothesis)).mean()
print (hypothesis >= torch.FloatTensor([0.5]))