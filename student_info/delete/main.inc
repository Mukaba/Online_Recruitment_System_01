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
 | Filename: main.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: This file allows the director to delete a student. Author:      |
 | -CHaN                                                                        |
 +------------------------------------------------------------------------------+

*/
?>
</td><td align="left"><img src="misc/images<?php echo $_SESSION['SESSION_colorscheme']; ?>/topbar_right.jpg" alt='' /></td></tr></table>
<h3>Student Information - Delete Student</h3>
<br />
<?php
    print "<table border='0' cellpadding='2' cellspacing='0'>";
        print "<tr align='center'><td class='row1'>";
            print "<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>";
                print "<form name='myform' action='".$PHP_SELF."&amp;select=verify_delete_student' method='post'>";
                print "<tr>";
                    print "<td align='right'>Student Number:</td>";
                    print "<td><input name='student_num' type='text' /></td>";
                print "</tr>";
                print "<tr>";
                    print "<td align='right'>Department:</td>";
                    print "<td><select name='dept_id'>";

                    $sql = "SELECT department_name,department_id
                            FROM department
                            ORDER BY department_name";
                    $results = $GLOBALS['dbh']->Execute($sql);
                    if ($results->RecordCount() > 0) 
                    {
                        while($row=$results->FetchRow())
                        {
                            print "<option value='".$row['department_id']."'>".$row['department_name']."</option>";
                        }
                    }
                    print "</select></td>";
                print "</tr>";
                print "<tr>";
                    print "<td colspan='2'>";
                        print "<hr />";
                    print "</td>";
                print "</tr>";
                print "<tr>";
                    print "<td>&nbsp;</td>";
                    print "<td><input type='submit' value='Prepare Delete' /></td>";
                print "</tr>";
                print "</form>";
            print "</table>";
        print"</tr>";
    print "</table>";
?>
