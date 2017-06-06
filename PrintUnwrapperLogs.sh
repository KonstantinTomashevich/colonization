for logFile in UnwrapperLogs/* 
do
    echo "${logFile}:"
    cat ${logFile}
    echo ""
done
