# create
import random

arry = [
"Sophia","Jackson","Emma","Aiden","Olivia","Lucas","Ava","Liam","Mia","Noah","Isabella","Ethan","Riley","Mason","Aria","Caden","Zoe","Oliver","Charlotte","Elijah","Lily","Grayson","Layla","Jacob","Amelia",
"Michael","Emily","Benjamin","Madelyn","Carter","Aubrey","James","Adalyn","Jayden","Madison","Logan","Chloe","Alexander","Harper","Caleb","Abigail","Ryan","Aaliyah","Luke","Avery","Daniel","Evelyn","Jack",
"Kaylee","William","Ella","Owen","Ellie","Gabriel","Scarlett","Matthew","Arianna","Connor","Hailey","Jayce","Nora","Isaac","Addison","Sebastian","Brooklyn","Henry","Hannah","Muhammad","Mila","Cameron","Leah",
"Wyatt","Elizabeth","Dylan","Sarah","Nathan","Eliana","Nicholas","Mackenzie","Julian","Peyton","Eli","Maria","Levi","Grace","Isaiah","Adeline","Landon","Elena","David","Anna","Christian","Victoria","Andrew","Camilla",
"Brayden","Lillian","John","Natalie","Lincoln"
]


for i in range(100):
    string = "input_" + str(i) + ".txt"
    with open (string, 'w') as f:
        #print "now printing"
        for j in range(100):
            num = random.randint(1,95)
            print num
            name = arry[num]
            price = round(random.uniform(-100000.0,100000.0), 2)
            credDebtNum = random.randint(1,2)
            credDebt = "debit"
            if credDebtNum == 1:
                credDebt = "credit" # assign -/+

            word = name + ' ' + credDebt + ' $' + str(price) + '\n'
            f.write(word)
