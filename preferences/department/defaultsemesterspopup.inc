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
 | Filename:    DefaultSemestersPopup.php                                       |
 +------------------------------------------------------------------------------+
 | Description: This is the popup to edit default timetables per department.    |
 +------------------------------------------------------------------------------+

*/

$department_id = $auth->department;

//start decision process
switch ($semesters_update)                                                          //set in semester_form.inc
{
    case "Save":
        $sql = "SELECT department_id 
                FROM department_default_semesters
                WHERE department_id = '$department_id'";
        
        $result = $GLOBALS['dbh']->Execute($sql);
        
        if( $result->RecordCount() > 0 )
        {
             $no_tt_entry = false;       //there already exists saved timetables for this department  
        } 
        else
        {
             $no_tt_entry = true;        //there are no saved timetables for this department
        }
        
        for( $tt_count = 0; $tt_count <= 2; $tt_count++ )                           //create each timetable
        {
            for( $year_count = 0; $year_count <= 5; $year_count++ )                 //display 6 years worth of terms
            {
                for( $term_count = 0; $term_count <= $num_terms-1; $term_count++ )  //for each term display a selection box
                { 
                    if( $no_tt_entry )
                    {
                        $sql = "INSERT INTO department_default_semesters
                                ( department_id, timetable_num, term_id, year_num, semesters_id )
                                VALUES
                                ( $department_id, 
                                  $tt_count, 
                                  $term_count, 
                                  $year_count,".
                                  $default_semesters[$tt_count][$year_count][$term_count]." )";
                    }
                    else
                    {
                        $sql = "UPDATE department_default_semesters 
                                SET semesters_id = '".($default_semesters[$tt_count][$year_count][$term_count])."'
                                WHERE department_id = '$department_id'
                                AND timetable_num = '$tt_count'
                                AND term_id = '$term_count'
                                AND year_num = '$year_count'";
                    }
                    
                    $result = $GLOBALS['dbh']->Execute($sql); 
                }
            }
        }
    
        //No need for break; here as we need to close the window anyway after we have saved.
    
    case "Cancel":
?>

        <script type='text/javascript' language='javascript'>
            window.close();
        </script>

<?php
        break;
    
    case "Edit":
    default:
        include("preferences/department/semesters_form.inc");
        break;
}

?>
