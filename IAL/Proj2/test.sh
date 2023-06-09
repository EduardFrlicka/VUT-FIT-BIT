#!/bin/sh
#---------------------------------------INFO-------------------------------------------------------------------

# Script for project testing
# Help: ./test.sh --help
# by Gargi

# User options:

EXECSUFFIX="-test"      #executable files suffix
EXECEXT=""              #executable files extension
OUTEXT=".output"        #output files extension
MYSUFFIX="-my"          #my output suffix
MAKEOPT=""              #makefile options or dependencies

#--------------------------------------SCRIPT------------------------------------------------------------------

SKIP=0
CLEAN=0
DIRFIND="$PWD"
DIRARG=""
CODE=0

errorexit() { 
    printf "Error: %s\n" "$*" >&2
    exit $CODE
}

while [ $# -gt 0 ]; do
    case $1 in
        --help)
            printf "Script for project testing ~ by Gargi\n"
            printf "Usage: ./test.sh [OPTION]... [NAME]\n\n"
            printf "Basic operations make, memory, tests for projects testing.\n"
            printf "Executing without any options script run tests in all directories in current directory.\n"
            printf "With NAME passed in arguments, script will run tests only once in 'NAME' directory.\n"
            printf "Directory NAME must be same as executable file (without suffix and extension) made by Makefile.\n"
            printf "Custom file suffix, extensions or options can be modified at start of script file.\n\n"
            printf "Options list:\n"
            printf "  --clean\t clean files with 'make clean' and delete files created by this script (in NAME directory)\n"
            printf "  --dir [DIR]\t directory where to look for directories for testing\n"
            printf "  \t\t   without this option, current directory will be used\n"
            printf "  --skip\t skip output compare\n"
            printf "  --help\t display this help and exit\n\n"
            printf "Exit status:\n"
            printf " 0 if OK\n"
            printf " 1 if problems with script options or directory\n\n"
            printf "Color output:\n"
            printf "cyan\tdirectory name\n"
            printf "green\tcorrect test\n"
            printf "yellow\tmaybe incorrect test\n"
            printf "red\tincorrect test\n\n"
            printf "Packages used: make, valgrind, grep, awk\n"
            exit $CODE
            ;;
        --clean)
            if [ $CLEAN -eq 1 ]; then
                CODE=1
                errorexit "Option '$1' was already set"
            fi
            CLEAN=1
            shift
            ;;
        --skip)
            if [ $SKIP -eq 1 ]; then
                CODE=1
                errorexit "Option '$1' was already set"
            fi
            SKIP=1
            shift
            ;;
        --dir)
            if [ "$DIRFIND" != "$PWD" ]; then
                CODE=1
                errorexit "Option '$1' was already set"
            elif [ "$2" = "--help" ]; then
                shift
                continue
            elif [ -z "$2" ] || [ "$2" = "--clean" ] || [ "$2" = "--skip" ]; then
                CODE=1
                errorexit "Option '$1' requires an argument"
            elif [ ! -d "$2" ]; then
                CODE=1
                errorexit "Directory '$2' does not exist"
            fi
            DIRFIND=$2
            cd "$DIRFIND" || CODE=1 | errorexit "You do not have permissions to enter '$DIRFIND' directory"
            shift
            shift
            ;;
        *)
            if [ -n "$DIRARG" ]; then
                CODE=1
                errorexit "Option '$1' does not exist or test directory was already set to '$DIRARG'"
            fi
            DIRARG=$(basename "$1") 
            shift
            ;;
    esac
done

DIRS=$(find * -maxdepth 0 -type d 2>/dev/null)
if [ ! -z $DIRARG ] && [ ! -d "$DIRARG" ]; then errorexit "Test directory '$DIRARG' does not exist"; fi
if [ -z "$DIRS" ]; then errorexit "No test directories found in '$DIRFIND'"; fi

echo "$DIRS" | while IFS= read -r DIR; do 

    if [ -z "$DIRARG" ] || [ "$DIR" = "$DIRARG" ]; then

        printf "\n"
        echo "======================================================================"
        echo "Directory: $DIR"

        cd "$DIR" 2>/dev/null 
        EXTCODE=$?
        if [ "$EXTCODE" -ne 0 ]; then
            echo "You do not have permissions to enter '$DIR' directory" >&2
            continue
        fi

        echo "-------------------make-------------------"

        if [ ! -f "Makefile" ]; then 
            echo "Directory does not contain Makefile" >&2
            cd ..
            continue
        fi

        make clean >/dev/null
        EXTCODE=$?
        if [ "$EXTCODE" -eq 0 ]; then 
            echo "Make clean: ok "
        elif [ "$EXTCODE" -eq 2 ]; then
            echo "Make clean: \e[1;33mmissing "
        else 
            echo "Make clean: error "
        fi

        if [ "$CLEAN" -eq 1 ]; then

            echo "--------------------rm--------------------"

            if [ -f "$DIR""$MYSUFFIX""$OUTEXT" ]; then
                rm -f "$DIR""$MYSUFFIX""$OUTEXT" 
                echo "Delete $DIR$MYSUFFIX$OUTEXT: ok "
            else 
                echo "Delete $DIR$MYSUFFIX$OUTEXT: \e[1;33mnot found "
            fi

            cd ..
            if [ -n "$DIRARG" ]; then break; fi
            continue
        fi 

        if [ -z "$MAKEOPT" ]; then
            make >/dev/null
            EXTCODE=$?
        else 
            make "$MAKEOPT" >/dev/null
            EXTCODE=$?
        fi
        
        if [ "$EXTCODE" -eq 0 ]; then
            echo "Make executable file: ok "
        else
            printf "\n"
            echo "Make executable file: error "
            cd ..
            continue
        fi

        if [ ! -f "$DIR""$EXECSUFFIX""$EXECEXT" ]; then 
            echo "Executive file '$DIR$EXECSUFFIX$EXECEXT' does not exist" >&2
            cd ..
            continue
        fi
		
        echo "-----------------valgrind-----------------"

        valgrind -q ./"$DIR""$EXECSUFFIX""$EXECEXT" 2>&1 | grep -v 'error calling PR_SET_PTRACER, vgdb might block' | grep '==[0-9][0-9][0-9][0-9]=='
        EXTCODE=$?
        if [ "$EXTCODE" -eq 1 ]; then
            echo "Memory access (alloc): ok "
        else 
            echo "Memory access (alloc): error "
        fi

        valgrind ./"$DIR""$EXECSUFFIX""$EXECEXT" 2>&1 | grep 'All heap blocks were freed -- no leaks are possible' >/dev/null
        EXTCODE=$?
        if [ "$EXTCODE" -eq 0 ]; then
        	echo "Memory leaks (free): ok " 
        else
            valgrind ./"$DIR""$EXECSUFFIX""$EXECEXT" 2>&1 | awk '/HEAP/,/suppressed: .+ blocks$/'
            echo "Memory leaks (free): error " 
        fi

        echo "-------------------test-------------------"

        if [ "$SKIP" -eq 1 ]; then
            echo "Output files: \e[1;33mskipped "
        else 
            ./"$DIR""$EXECSUFFIX""$EXECEXT" >"$DIR""$MYSUFFIX""$OUTEXT"

		    diff -q "$DIR"*"$OUTEXT" >/dev/null 2>&1
            EXTCODE=$?
            if [ "$EXTCODE" -eq 0 ]; then
                echo "Output files: ok "
            else 
                diff -u "$DIR"*"$OUTEXT"
                echo "Output files: not same "
            fi
        fi

        cd ..
        if [ -n "$DIRARG" ]; then break; fi
        continue
    fi
done

printf "\n"
exit "$CODE"