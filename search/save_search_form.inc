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
 | Filename: save_search_form.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This file has a form which is used to save the name of a        |
 | search and its description                                                   |
 +------------------------------------------------------------------------------+

*/


if($continue=="Edit Search Name/Description" || $continue=="Edit Search Criteria")  //If the user is coming from "Manage Searches"
{
    if (strlen($search_id))
    {
        $sql = ("
            SELECT s.search, s.search_name, s.search_description, sm.module_id
            FROM search AS s
            INNER JOIN search_module AS sm
            ON s.search_module_id = sm.module_id
            WHERE s.search_id = '".$search_id."'
            ");

        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        
        // When a user is just editing a search name and description, there's no need to update the search object, so we
        // unset the search object. 
        if ($continue == "Edit Search Name/Description")
        {
            $search = $row["search"];
        }
        $search_name = $row["search_name"];
        $search_description = $row["search_description"];
        $module_id = $row["module_id"];
    }

    if($continue=="Edit Search Name/Description")                                   //if its Edit Search the header is not set
    {
?>

        </td>
        <td align="left">
            <img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>">
        </td>
        </tr>
        </table>
        
<?php 
        echo("<h3 align='left'>Edit Search Name/Description</h3>");
        
    }
    else
    {
        if($module_id == HISTORY_ADV_SEARCH)  //Placement History's header is added here due to its structure
        {
            echo("<h3 align='left'>Placement History - Save Search</h3>");
        }
    }
    
    echo("<form action='$PHP_SELF' method='post'>");
    echo("<input type='hidden' name='search' value='$search' />");
    echo("<input type='hidden' name='search_id' value='$search_id' />");
    echo("<input type='hidden' name='module_id' value='$module_id' />");
    echo("<input type='hidden' name='search_description' value='$search_description' />");
}
else    //else if its the first time the search is being saved
{
    echo("<form action='$PHP_SELF' method='post'>");
    echo("<input type='hidden' name='select' value='save_search' />");
    echo("<input type='hidden' name='module_id' value='$module_id' />");
    echo("<input type='hidden' name='search' value='$search' />");

    if($module_id == HISTORY_ADV_SEARCH)
    {
        echo("<h3 align='left'>Placement History - Save Search</h3>");
    }
}


echo("<table border='0' cellpadding='2' cellspacing='0'>");
    echo("<tr class='row1'>");
        echo("<td>");
        echo("<input type='hidden' name='select' value='manage_searches' />");
            echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>");
                echo("<tr align='left'>");
                    echo("<td>");
                        echo("Search Name: &nbsp;");
                    echo("</td>");
                    echo("<td>");
                        echo("<input type='text' name='search_name' maxlength='100'  value='$search_name' />");
                    echo("</td>");
                echo("</tr>");
                echo("<tr align='left'>");
                    echo("<td>");
                        echo("Search Description: &nbsp;");
                    echo("</td>");
                    echo("<td>");
                        echo("<textarea cols='70' rows='6' name='search_description'>");
                            echo($search_description);                         //This dumps the description from database into the text field if it exists
                        echo("</textarea>");
                    echo("</td>");
                echo("</tr>");
            echo("</table>");

            echo("<br /><center><input type='submit' value='Save' /></center>");
        echo("</td>");
    echo("</tr>");
   echo("<tr>");  
   echo("<td>"); 
    echo("<input type='hidden' name='continue' value='Check Unique Name' />");   //Determine whether or not the submitted search name is in the database
 echo("</td>");
 echo("</tr>"); 
echo("</table>");
echo("</form>");

?>

