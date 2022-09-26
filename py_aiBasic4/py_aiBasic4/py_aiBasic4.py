import torch
import torch.optim as optim

#I Set 6 rank vector, for 6 carps.
x_train = torch.FloatTensor([[40], [50], [60], [70], [75], [80]])
y_train = torch.FloatTensor([[0.5], [1.375], [2.25], [3.125], [3.5625], [4]])

W = torch.zeros(1, requires_grad=True)
b = torch.zeros(1, requires_grad=True)
#Reduce lr to prevent divergence.
optimizer = torch.optim.SGD([W, b], lr=0.0002)

#For reduce cost, I increased the number of lessons.
nb_epochs = 50000
for epoch in range(nb_epochs + 1):
    hypothesis = x_train * W + b
    cost = torch.mean((hypothesis - y_train) ** 2)
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    if epoch % 5000 == 0:
        print('Epoch {:4d}/{} W: {:.3f}, b: {:.3f} Cost: {:.6f}'.format(epoch, nb_epochs, W.item(), b.item(), cost.item()))