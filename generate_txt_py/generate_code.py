import json 

problemId = 0
# ADD HOLDS FOR YOUR SPECIFIC MOONBOARD SET 
setB = [
	"H16",
	"B15",
	"J12",
	"J13",
	"K16",
	"F13",
	"E16",
	"I7",
	"I15",
	"I9",
	"E12",
	"H5",
	"G15",
	"J7",
	"H12",
	"G17",
	"E9",
	"J16",
	"F11",
	"D11",
	"I10",
	"K9",
	"E8",
	"A14",
	"I14",
	"C5",
	"D15",
	"E14",
	"G9",
	"E6",
	"J5",
	"H8",
	"I18",
	"A9",
	"G6",
	"C8",
	"D18",
	"G14",
	"C13",
	"A18",
	"H10",
	"G4",
	"C13",
	"A18",
	"H10",
	"G4",
	"B12",
	"C16",
	"K14",
	"G10",
	"D7",
	"B6",
	"B10",
	"H13"
]

with open("filtered_problems.json", "w") as filtered_problems:
    p_filtered = { }
    with open('STM32_code.txt', 'w') as output:
        # switch / case structure for STM32
        output.write('void createProblem(Problem* p, int id){\n')
        output.write('\tswitch(id){\n')

        with open('moonboard_problems_setup_2016.json', 'r') as openfile:
            jObj = json.load(openfile)
            for p in jObj:
                inSet = True
                # check if the problem is in set 
                moves = jObj[p]["Moves"]
                for m in moves:
                    if m["Description"] not in setB:
                        inSet = False
                        break
                # if the problem is doable, write on the output text and filtered_problems
                if inSet:
                    problemId+=1 
                    name = jObj[p]["Name"]
                    grade = jObj[p]["Grade"]
                    sended = False
                    # writing on the txt file 
                    output.write(f'\t\tcase {problemId} :\n')
                    output.write('\t\t\tstrcpy(p->name,"' + name +'"); \n')
                    output.write('\t\t\tstrcpy(p->grade,"' + grade +'"); \n')
                    output.write(f'\t\t\tp->moveSize = {len(moves)};\n' )
                    moveLetters = ""
                    moveNumbers = ""
                    startFinish = ""
                    moveLetters2 = ""
                    moveNumbers2 = ""
                    startFinish2 = ""
                    for m in moves:
                        md = m["Description"]
                        ms = m["IsStart"]
                        me = m["IsEnd"]
                        moveLetters += (md[0] + ",")
                        if len(md) == 2:
                            moveNumbers += (md[1] + ",")
                        elif len(md) == 3: 
                            moveNumbers += (md[1] + md[2]+ ",")
                        if ms:
                            startFinish += "s,"
                        elif me:
                            startFinish += "e,"
                        else: startFinish += "d,"
                    # deleting last comma
                    for i in range(len(moveLetters)-1):
                        moveLetters2 += moveLetters[i]
                    for i in range(len(moveNumbers)-1):
                        moveNumbers2 += moveNumbers[i]
                    for i in range(len(startFinish)-1):
                        startFinish2 += startFinish[i]
                    # keep writing 
                    output.write('\t\t\tstrcpy(p->moveLetters,"' + moveLetters2 +'");\n')
                    output.write('\t\t\tstrcpy(p->moveNumbers,"' + moveNumbers2 +'");\n')
                    output.write('\t\t\tstrcpy(p->startFinish,"' + startFinish2 +'");\n')
                    output.write('\t\t\tbreak;\n')

                    # write in new file json 
                    p_filtered[problemId] = jObj[p] 
                    p_filtered[problemId]["Sended"] = False 

    # write on filtered JSON file 
    jObj_filtered = json.dumps(p_filtered, indent=4)        
    filtered_problems.write(jObj_filtered)
    print (f"Iteration completed. N of problems {problemId}")
