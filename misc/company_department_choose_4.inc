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
   | Filename: company_department_choose_4.php                                    |
   +------------------------------------------------------------------------------+
   | Description: based on company_choose.php, this file allows you to choose     |
   | only a company that a division.                                              |
   +------------------------------------------------------------------------------+ 
*/
?>

<script type='text/javascript' language='javascript'>

<!--javascript

function unfocus()
{
        self.close();
}

function fillCompanyName(companyName)
{
    var x = document.form1.company_slot.value;
    var formName = document.form1.parentFormName.value;
    arrNames = companyName.split(":");
    if (arrNames.length==1){
        arrNames[1]='';
    }
    if(arrNames[1] !=''){
        str = "window.opener.document." + formName + ".elements[" + x + "].value = arrNames[0]";
        eval(str);
        <?php
            if ($autosubmit == "true") {                                                                                                                 
                print "\tx=x+1;\n";
            }
            else {
                print "\tx=x+2;\n";
            }
        ?>
            <?php
            if ($autosubmit == "true") {
                ?>
                    y=x+1;
                str = "window.opener.document.myform.changed_company_division.value = '1'";
                eval(str);
                <?php
            }
        ?>
            str = "window.opener.document." + formName + ".elements[" + x + "].value = arrNames[1]";
        eval(str);
        <?php
            if ($autosubmit == "true") {
                print "\tstr = \"window.opener.document.\" + formName + \".submit()\";\n";
                print "\teval(str);\n";
            }                                                                                                                                            
        ?>
            self.close();
    }
}                                                                                                                                                    

function resizeOuterTo(w,h)                                                                                                                          
{                                                                                                                                                    
    <?php                                                                                                                                            
        $client = new wwwclient;                                                                                                                         
    $client->wwwclient();                                                                                                                            
    ?>                                                                                                                                               
        var browser = "<?php echo $client->browser; ?>";                                                                                                 

    var pattern = /X11/;                                                                                                                             
    var result;                                                                                                                                      
    result=pattern.exec(navigator.appVersion);                                                                                                       
    if (result != null)                                                                                                                              
    {                                                                                                                                                
        window.resizeTo((w + 320),(h + 120));                                                                                                        
    }                                                                                                                                                

    w = document.getElementById('CDC4').offsetWidth;                                                                                                 
    h = document.getElementById('CDC4').offsetHeight;                                                                                                

    if (browser == <?php echo NETSCAPE; ?>)                                                                                                          
    {                                                                                                                                                
        w = w + 30;                                                                                                                                  
        h = h + 50;                                                                                                                                  
    }                                                                                                                                                
    if(browser == <?php echo IE; ?>)                                                                                                                 
    {                                                                                                                                                
        w = w + 40;                                                                                                                                  
        h = h + 30;                                                                                                                                  
    }                                                                                                                                                
    if(browser == <?php echo GECKO; ?>)                                                                                                              
    {                                                                                                                                                
        // w = w + 200;                                                                                                                              
        //h = h + 90;                                                                                                                                
    }                                                                                                                                                
    if(browser == <?php echo LYNX; ?>)                                                                                                               
    {                                                                                                                                                
        //w = w + 150;                                                                                                                               
        //h = h + 70;                                                                                                                                
    }                                                                                                                                                
    if(browser == <?php echo SAFARI; ?>)                                                                                                             
    {                                                                                                                                                
        w = w + 40;                                                                                                                                  
        h = h + 50;                                                                                                                                  
    }                                                                                                                                                
    if(browser == <?php echo FIREFOX; ?>)                                                                                                            
    {                                                                                                                                                
        w = w + 20;                                                                                                                                  
        h = h + 30;                                                                                                                                  
    }                                                                                                                                                
    else                                                                                                                                             
    {                                                                                                                                                
        //w = w + 150;                                                                                                                               
        //h = h + 100;                                                                                                                               
    }                                                                                                                                                
    if(navigator.platform == "Win32")                                                                                                                
    {                                                                                                                                                
        w = w + 10;                                                                                                                                  
        h = h + 30;                                                                                                                                  
    }                                                                                                                                                
    window.resizeTo(w,h);                                                                                                                            
}                                                                                                                                                    

//-->                                                                                                                                                

</script>                                                                                                                                            
    

<?php

if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{  
    $sql = ("
            SELECT DISTINCT SUBSTRING(company_name, 1, 1) AS order_letter
            FROM employer_company
            WHERE company_name IS NOT NULL
            AND company_name!=''
            AND company_display
            ORDER BY company_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        $order_choices[] = strtoupper($row["order_letter"]);
    }

    if (!$order)
    {
        $order = $order_choices[0];
    }
    if ($autosubmit == "true") 
    {                                                                                                                     
        $order_href = ("mamook.php?select=company_department_choose_4&no_headers=1&company_slot=" . $company_slot . "&parentFormName=" . $parentFormName . "&autosubmit=true");   
    }                                                                                                                                                
    else
    {                                                                                                                                           
        $order_href = ("mamook.php?select=company_department_choose_4&no_headers=1&company_slot=" . $company_slot . "&parentFormName=" . $parentFormName);                        
    }   
    
    $sql = ("
            SELECT DISTINCT ec.employer_id, ec.company_name,concat(ed.department_name,\" (\",ed.city,\")\") as d_name,ed.department_name,ed.department_id, ed.city,
            ec.city as eccity, concat(ec.company_name,\" (\",ec.city,\")\") as c_name
            FROM employer_company as ec 
            LEFT JOIN employer_department as ed
            ON ed.employer_id=ec.employer_id
            WHERE ec.company_name IS NOT NULL
            AND ec.company_display=1
            AND ec.company_name!=''
            AND ec.company_name LIKE '" . $order . "%'
            AND ec.company_display
            ORDER BY ec.company_name, ed.department_name                                                                                             
            ");
    $result = $GLOBALS['dbh']->Execute($sql);


    echo("<form name='form1'>");
    
    echo("<input type='hidden' id='company_slot' value='" . $company_slot . "'>");                                                                 
    echo("<input type='hidden' id='parentFormName' value='" . $parentFormName . "'>"); 

    echo("<center>");
    echo("<table border='0' width='100%' height='100%' align='center' valign='middle'>");
    echo("<tr align='center'>");
    echo("<td align='center'>");
    echo("<table id='CDC4' border='0' cellpadding='5' cellspacing='0' class='row1'>");
    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<br />");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td align='center' colspan='2' nowrap>Choose a company/division:</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'>");
    echo("<table width='100%' cellpadding='5' cellspacing='5' border='0'>");
    echo("<tr>");
    echo("<td colspan='2' class='row0'>");
    echo("<table width='96%' cellpadding='2' cellspacing='0' border='0'>");
    echo("<tr>");
    echo("<td colspan='2' align='center' nowrap>");
    for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
    {
        echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a> | ");
    }
    echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>" . $order_choices[$i] . "</a>");
    echo("<input type='hidden' name='order_width' value='" . sizeof($order_choices) . "'>");
    echo("</td>");
    echo("</tr>");
    echo("<tr>");
    echo("<td colspan='2' align='center' >");
    $size = 10;
    if ($result->RecordCount() < 10)
    {
        $size = $result->RecordCount() + 1;
    }
    if ($result->RecordCount() > 0)
    {
        echo("<select name='company_name' onChange='fillCompanyName(document.form1.company_name[document.form1.company_name.selectedIndex].value)' size='" . $size . "'>");
        echo("<option value='' ");
        if (!$company_name || $company_name == '')
        {
            echo("selected");
        }
        echo(">&nbsp;</option>");
        while ($row = $result->FetchRow())
        {
            if ($lastCompanyName<>$row['company_name']){
                echo("<option value=\"" . htmlentities($row["company_name"]) . "\" ");
                if ($employer_id == $row["company_name"])
                {
                    echo("selected");
                }
                echo(">");
                echo ($row["company_name"] . "</option>");
            }
            // For department names, we concatenate a city to the department names. However, concatentation fails if we try to
            // concatenate a NULL city. Therefore, we check to make sure that the city is not null, and we provide additional
            // cases to handle this problem.
            if ($row['department_id']<>NULL && $row['city']<>NULL){
                echo ("<option value=\"".htmlentities($row['company_name']).":".htmlentities($row['department_name'])."\">&nbsp;&nbsp;-".$row['d_name']."</option>");
            }
            elseif ($row['department_name'] <> NULL) {
                echo ("<option value=\"".htmlentities($row['company_name']).":".htmlentities($row['department_name'])."\">&nbsp;&nbsp;-".$row['department_name']."</opt
                        ion>");
            }
            elseif ($row['employer_id'] <> NULL && $row['eccity'] <> NULL) {
                echo ("<option value=\"".htmlentities($row['company_name']).":".htmlentities($row['company_name'])."\">&nbsp;&nbsp;-".$row['c_name']."</option>");
            } 
            elseif ($row['company_name'] <> NULL) {
                echo ("<option value=\"".htmlentities($row['company_name']).":".htmlentities($row['company_name'])."\">&nbsp;&nbsp;-".$row['company_name']."</option>")
                    ;               
            }
            $lastCompanyName=$row['company_name'];


        }
        echo("</select>");
    }
    else
    {
        echo("<br />No companies starting with '" . $order . "'.<br /><br />");
    }
    echo("</td>");
    echo("</tr>"); 
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</form>");
}
else 
{
    echo("</head><body bgcolor='#ffffff'>");
}
?>

<script type='text/javascript' language='javascript'>

var heightOffSet = 0;
var widthOffSet = 0;

var widthMultiplier = 8;                                                                                                                             
var heightMultiplier = 10;                                                                                                                           

var selectLength = 0;

selectLength = (document.form1.company_name.length - 2);                                                                                             

if (selectLength > 10)                                                                                                                               
{                                                                                                                                                    
    selectLength = 10;                                                                                                                               
}

var selectWidth = 0;                                                                                                                                 

var i;

for (i = 0; i < document.form1.company_name.length; i++)
{
    curLength = document.form1.company_name[i].text.length;
    if (curLength > selectWidth)
    {
        selectWidth = curLength;
    }
}

selectWidth = selectWidth - 30;

if (selectWidth < 0)
{
    selectWidth = 0;
}

if (document.form1.order_width.value > selectWidth)
{
    selectWidth = document.form1.order_width.value;
    widthMultiplier = 9.0;                                                                                                                           
} 

widthOffSet = document.getElementById('CDC4').offsetWidth;                                                                                           
heightOffSet = document.getElementById('CDC4').offsetHeight;                                                                                         

resizeOuterTo((widthOffSet),(heightOffSet));                                                                                                         

</script>                                                                                                                                            
</body>    
</html>
<?php
ob_end_flush();
?>
