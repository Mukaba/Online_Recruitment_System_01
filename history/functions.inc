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
 | Filename: functions.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: This file holds functions related to history                    |
 +------------------------------------------------------------------------------+

*/
function history_flags($history_id,$department_id)
{
	$marked_flags_sql = "
            SELECT hf.history_flags_id as id, description
            FROM history_flags AS hf, 
                 history AS h, 
                 history_flags_join AS hfj, 
                 department_history_flags AS dhf 
            WHERE h.history_id=hfj.history_id 
              AND dhf.history_flags_id=hf.history_flags_id 
              AND hf.history_flags_id=hfj.history_flags_id 
              AND dhf.department_id='".$department_id."' 
              AND h.history_id='".$history_id."'
            ORDER BY hf.description";    	
	$marked_flags_result=$GLOBALS['dbh']->Execute($marked_flags_sql);
    // GetAssoc relys on history_id being the first row so it can use it as a key.  Dont change the order.
	$marked_flags = $marked_flags_result->GetAssoc();
	
    $all_flags_sql = "
             SELECT hf.history_flags_id as id, description
             FROM history_flags as hf,
                  department_history_flags as dhf
             WHERE dhf.history_flags_id=hf.history_flags_id 
               AND dhf.department_id=".$department_id." 
             ORDER BY hf.description";
	$all_flags_result=$GLOBALS['dbh']->Execute($all_flags_sql);
    // GetAssoc relys on history_id being the first row so it can use it as a key.  Dont change the order.
    $all_flags = $all_flags_result->GetAssoc();
    
    foreach ($all_flags as $id => $description){
        $flags[$id]['description'] = $description;
        $flags[$id]['checked'] = (isset($marked_flags[$id]));
    }
    return $flags;
}

function history_search_flags($departmentid)
{
    $anarray = array();

    $count=0;
    $sql = ("
        SELECT hf.description, hf.history_flags_id
        FROM history_flags AS hf
        INNER JOIN department_history_flags AS dhf
        ON hf.history_flags_id = dhf.history_flags_id
        WHERE dhf.department_id='".$departmentid."'
        ORDER BY hf.description, hf.history_flags_id
        ");

    $results=$GLOBALS['dbh']->Execute($sql);
    while ($row=$results->FetchRow())
    {
        $anarray[$count+1] = "&nbsp";
        $anarray[$count]=$row["description"];
        $count=$count+2;
    }

    return $anarray;
}

?>
