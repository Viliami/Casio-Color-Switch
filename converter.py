lines = int(input("How many lines?"))
inputs = []

for i in range(lines):
    inputs.append(input(str(i)+": "))

y = 0
for i in inputs:
    i = i.replace(",", "")
    x = 0
    for number in i:
        if(number == "1"):
            print("draw_pixel(x+"+str(x)+", y+"+ str(y)+", color);")
        x+=1
    y+=1    
