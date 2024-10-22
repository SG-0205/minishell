awk
{
    # Le premier champ est le PID
    pid = $1
    
    # Vérifie si le fichier pour ce PID existe déjà, sinon le crée
    if (!(pid in files)) {
        files[pid] = pid ".log"
        print "Creating file: " files[pid]
    }
    
    # Écrit la ligne dans le fichier correspondant au PID
    print $0 >> files[pid]
}
fdebug.txt
