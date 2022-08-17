<?php

/*
 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
   | Filename: repair_database.php                                                |
   +------------------------------------------------------------------------------+                                                                    
   | Description: This script is used to check and repair database tables after   |
   |              some sort of abnormal termination.                              |
   +------------------------------------------------------------------------------+

   Note: Script usually takes a few minutes to run. 
         Results are stored in PATH_LOGS/repair_database.log if any repairs were needed
 */

    include('../release_constants.inc'); // release_constants to be included before constants.inc
    include('../constants.inc');  // constants must be included before wwwclient.inc                                                                   
    include('../names.inc');
    include('../functions.inc');
    include('../wwwclient.inc');
    include(PATH_ADODB . 'adodb.inc.php');
    include('../db.inc');
    include('../resume_functions.inc');
    include('../special_flags_functions.inc');

    if ($_SERVER['SERVER_ADDR'] != $_SERVER['REMOTE_ADDR'] && !in_array($_SERVER['REMOTE_ADDR'],$IPS_FOR_DEBUG))                                         
    {
        exit("Security Violation");
    }
    else 
    {
        ob_start(); 
        echo("======================================\n");
        echo(date("F j, Y (g:i a)")."\n\n");

        $sql = ("
            SHOW TABLES
            ");
        $results = $GLOBALS['dbh']->Execute($sql);

        $num_funky_tables = 0;

        echo("Table Name\t\t\t\t\t\t\tError / Warning\n");

        // cycle through the database tables
        while ($row = $results->FetchRow()) {
            $table_name = $row[0];

            $check_sql = ("
                CHECK TABLE ".$table_name."
                ");
            $check_result = $GLOBALS['dbh']->Execute($check_sql);

            while ($check_row = $check_result->FetchRow()) {

                // check for any abnormal statuses 
                if ($check_row['Msg_type'] != "status") {

                    // if so, report it
                    echo($table_name."\t\t\t\t\t\t\t".$check_row['Msg_text']."\n");
                    $num_funky_tables++;

                    $repair_sql = ("
                        REPAIR TABLE ".$table_name."
                        ");
                    $GLOBALS['dbh']->Execute($repair_sql);
                }
            }

        }
        echo("\n");
        echo("Number of tables with errors: ".$num_funky_tables."\n");
        echo("======================================\n");

        if ($num_funky_tables) {
            $output = ob_get_contents();
            $fp = fopen(PATH_LOGS . "repair_database.log", "a");
            fputs($fp, $output);
        }

        ob_end_clean();
    }
?>
