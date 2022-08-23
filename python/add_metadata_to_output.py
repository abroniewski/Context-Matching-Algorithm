import pandas as pd

sail=[]
fish=[]
match_sail=[]
match=[]
right = []
wrong = []

x=[]
y=[]

final_df = pd.DataFrame()

data_type = ""

with open('output.txt', 'r') as f:
    
    for line in f.readlines():

        line = line.strip('\n')
        line = line.strip(' ')    
        line = line.strip('(')
        line = line.strip(')')

        if line == '':
            pass

        elif line in ["sail","fish","match_sail","match_fish","fin","rw"]:
            
            if len(x)!=0:
                
                # Flush condition 
                if type_data == "sail":
                    sail.append(x)
                    sail.append(y)
                    d = {"x": sail[0], "y": sail[1], "label": type_data}
                    final_df = pd.concat( [final_df, pd.DataFrame(d)] )
                if type_data == "fish":
                    fish.append(x)
                    fish.append(y)
                    d = {"x": fish[0], "y": fish[1], "label": type_data}
                    final_df = pd.concat( [final_df, pd.DataFrame(d)] )
                if type_data == "match_sail" :
                    match_sail.append(x)
                    match_sail.append(y)
                if type_data == "match_fish" :
                    match.append(x)
                    match.append(y)
                if type_data == "rw" :
                    right.append(x)
                    wrong.append(y)

            x=[]
            y=[]
            type_data = line

        else:
            line = line.split(",")
            x.append(float(line[0]))
            y.append(float(line[1]))


# print(sail_y)


final_df["id"] = "c827a6"
print(final_df)
final_df.to_csv("db_ready.csv", index=False)
