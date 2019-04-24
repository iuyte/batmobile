import jsonpickle, csv

class Commit(object):
    date = ""
    commitkey = ""
    author = ""
    description = ""
    filesModified = []
    filesAdded = []
    filesDeleted = []
    def __init__(self, date, commitkey, author, description, filesModified, filesAdded, filesDeleted):
        self.date = date
        self.commitkey = commitkey
        self.author = author
        self.description = description
        self.filesModified = filesModified
        self.filesAdded = filesAdded
        self.filesDeleted = filesDeleted

def printCommit(commit):
    frozen = jsonpickle.encode(commit)
    print(frozen)

class JSONStore(object):
    def __init__(self, commit):
        self.a_timestamp = commit.date
        self.b_author = "Ethan"
        self.c_description = commit.description
        self.d_changes = []
        for file in commit.filesAdded:
            self.d_changes.append("Added: " + file)
        for file in commit.filesDeleted:
            self.d_changes.append("Deleted: " + file)
        for file in commit.filesModified:
            self.d_changes.append("Modified: " + file)

def saveList(listOfCommits):
    with open("docs/log.csv", newline="", mode="w") as f:
        w = csv.writer(f, delimiter=';', quotechar='"', quoting=csv.QUOTE_MINIMAL)
        w.writerow(['Date', 'Description', 'Files Changed'])
        for c in listOfCommits:
            changes = c.filesAdded + c.filesModified + c.filesDeleted
            for i in range(len(changes)):
                ch = changes[i].split("/")
                changes[i] = ch[-1]
            w.writerow([c.date[6:12], c.description, ", ".join(changes)])

    f = open("docs/log.json", mode="w")
    for i in range(len(listOfCommits)):
        listOfCommits[i] = JSONStore(listOfCommits[i])
    toWrite = jsonpickle.encode(listOfCommits, unpicklable=False)
    toWrite = toWrite.replace("a_timestamp", "timestamp")
    toWrite = toWrite.replace("b_author", "author")
    toWrite = toWrite.replace("c_description", "description")
    toWrite = toWrite.replace("d_changes", "changes")
    f.write(toWrite)
    f.close()
