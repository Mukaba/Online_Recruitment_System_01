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
 | Filename: header.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: header.inc
// show Mamook[tm] logo and menu bar


//<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN"
// "http://www.w3.org/TR/REC-html40/loose.dtd">

if ($_REQUEST['no_html'] != 1){

?>
<!DOCTYPE html PUBLIC
"-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>
<head>
<!--
+==============================================================================+
| Mamook[tm] Software                                                          |
+==============================================================================+
| Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
+==============================================================================+
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
+==============================================================================+
-->
<?php
	if (SHOW_DB_ON_TITLE == "show_title")
		echo "<title>".ORGANIZATION_CODE." Co-op ".BRAND_NAME." [".DATABASE."]</title>\n";
	else
		echo "<title>".ORGANIZATION_CODE." Co-op ".BRAND_NAME."</title>\n";
  
	if ($client->platform == WIN)
	{
		echo("<link rel=\"stylesheet\" href=\"misc/css/font-win.css\" type=\"text/css\" />\n");
	}
	elseif ($client->browser == IE)
	{
		echo("<link rel=\"stylesheet\" href=\"misc/css/font-nonwin-ie.css\" type=\"text/css\" />\n");
	}
	elseif (($client->platform == LINUX) || ($client->platform == UNIX))
	{
		echo("<link rel=\"stylesheet\" href=\"misc/css/font-x.css\" type=\"text/css\" />\n");
	}
	else
	{
		echo("<link rel=\"stylesheet\" href=\"misc/css/font-nonwin.css\" type=\"text/css\" />\n");
	}
// Include Header tags below this point to ensure that the pages are not stored in a cache
?>
<meta http-equiv="Expires" content="-1" />
<meta http-equiv="Cache-Control" content="no-cache" />
<?php
/*
<meta http-equiv="Pragma" content="no-cache" />
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
*/
?>

<link rel="stylesheet" href="<?php echo $GLOBALS['colorscheme']['stylesheet']; ?>" type="text/css" />
<link rel="stylesheet" href="misc/css/spectrum.css" type="text/css" />
<?php

if ($_SESSION['SESSION_netlog']==SCRIPT_LOGIN){ 
    $sql = "SELECT user_font_size
            FROM contact_internal
            WHERE netlink_id='" . $login_id. "'";
} else {
    $sql = "SELECT user_font_size
            FROM contact_internal
            WHERE login_id='" . $login_id ."'";
}
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
if(empty($row['user_font_size'])){
    $fontsize='normal';
    //print "<link rel=\"stylesheet\" href=\"misc/css/font-".$fontsize.".css\" type=\"text/css\" />\n";
} else {
    $fontsize=$row['user_font_size'];
    print "<link rel=\"stylesheet\" href=\"misc/css/font-".$fontsize.".css\" type=\"text/css\" />\n";
}

//<link rel="SHORTCUT ICON" href="misc/images/phone.gif" />
?>
<link rel="home" href="mamook.php" />

<meta http-equiv="Expires" content="-1" />
<meta http-equiv="Cache-Control" content="no-cache" />

<script type="text/javascript" src="misc/global.js"></script>
<script type="text/javascript" src="misc/overlib/overlib.js"></script>

</head>

<body>
<?php

} // End if (!no_html)

// Now we check to see if we wanted headers or not (provided no_html wasnt set).
if ($_REQUEST['no_headers'] != 1 && $_REQUEST['no_html'] != 1){
?>

<!-- Include javascript popus -->
<script type="text/javascript">
var ol_border = 1;
var ol_bgcolor = '#000000';
var ol_fgbackground = "<?php echo $GLOBALS['colorscheme']['bg']; ?>";
var ol_fgcolor = "";
</script>

<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>
<script type="text/javascript" src="misc/overlib/overlib.js">
<!-- overLIB (c) Erik Bosrup -->
</script>

<table border="0" width="100%" cellspacing="0" cellpadding="0">
<tr align="left" valign="top"><td width="217" valign="top"><table cellpadding="0" cellspacing="0" border="0"><tr valign="top"><td>
		<?php
            $welcome_url = preg_replace("/(.*)\?.*/","\\1",$PHP_SELF); 
            echo("<a href='".$welcome_url."?select=welcome'>");
			echo("<img src='".$GLOBALS['colorscheme']['logo1']."' alt='Return to Front Page' border='0' width='217' height='111' />");
            echo("</a>");
		?>
</td></tr>
<tr valign="top"><td valign="top"><img src="<?php echo $GLOBALS['colorscheme']['menutop']; ?>" alt="" border="0" width="217" /></td></tr>
<tr valign="top"><td class="menuitem" valign="top"><?php include ("misc/show_menu.inc"); ?>
</td></tr>
<tr valign="top" class="bg">
    <td valign="top"><img src="<?php echo $GLOBALS['colorscheme']['menubottom']; ?>"  alt="" border="0" /></td></tr></table>

<center>
<a href='http://www.mamook.net' target="_blank"><img src="<?php print $GLOBALS['colorscheme']['poweredby']; ?>" alt="powered by <?php echo(BRAND_NAME); ?>" border="0" /></a></center><br /><br /></td>
<td valign="top" width="100%" align="left">
    <table cellpadding="0" cellspacing="0" border="0" class="topbar_bg">
        <tr valign="top" align="left"><td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_left']; ?>" alt="" /></td>
            <td valign="middle" align="center" nowrap="nowrap">&nbsp;
<?php } // End if (!no_header && !no_html) ?>
