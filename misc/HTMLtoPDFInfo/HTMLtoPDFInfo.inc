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
 | Filename: HTMLtoPDFInfo.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This is a file that gives a brief run down of instructions      |
 | for students created coverletters that will be converted to PDF files.       |
 +------------------------------------------------------------------------------+

*/
?>
<table width='80%' border='0'> 
	<tr>
		<td colspan='3'>
			<br /> <hr /><br />
		</td>
	</tr>

<!--
	<tr>
		<td colspan='3'>
			<h3>Updated Information!</h3>
		</td>
	</tr>
-->
	<tr>
	  	<td colspan='3' align='left'>
			<h3><b><u>HTML to PDF Information:</u></b></h3>
	
			<p><b>General Tag Info</b></p>
		<ul>
			<li>&lt;PRE&gt; Tag. Please refrain from using the &lt;PRE&gt; tag as it is not supported by the PDF conversion software.</li>
			<li>Tag order does matter. When opening tags appear in a certain order, the closing tags must appear in the opposite order. For example,
			if the opening bold tag appears before an opening underline tag, the closing underline tag must appear before the closing bold tag 
			(&lt;b&gt;&lt;u&gt;Title or Something&lt;/u&gt;&lt;/b&gt;).</li>
			<li>Almost all HTML tags MUST include their respective closing tag. For example, list items &lt;li&gt;, must
			be accompanied by a closing list tag &lt;/li&gt;. Furthermore paragraph tags &lt;p&gt; should also include their closing tag &lt;/p&gt;</li>
			<li>LISTS: If you have listed items do not include a paragraph tag if you would like the info on the same line. Inserting a paragraph tag will
			create a new paragraph within the list.</li>
			<li>MORE LISTS: Make sure that you when you define a list item, you have previously defined a list for all items. You MUST include the &lt;UL&gt;, &lt;OL&gt;, &lt;DL&gt;
			tags, along with their respective closing tags, when creating a list.</li>
			<li>PAGE BREAKS: If you would like to add page breaks for your PDF, insert &lt;!--PAGE BREAK--&gt; at the appropriate spot.</li>
			<li>IMAGES: Currently images are not placed into the PDF conversion. This is a server issue that is under investigation.</li>
			

		</ul>
		
			<p><b>Tables</b></p>
		<ul>
			<li>Because of the strictness of our program that  converts HTML to PDF, you must explicitly declare the widths of the columns. For example:
		  	if you have 2 columns in a table with a specified width and only declare one as being 15% wide, you should then also declare the other as 85% wide (which is done
		  	in the "TD" tag).</li>
			<li>Please keep in mind: Just because it looks really nice in HTML, doesn't mean it will look nice when printed out on paper. Paper sizes, margin
			widths, and web browsers aren't uniform for all employers when printing; PDF is the only way to ensure that all employers see the same thing.</li>
			<li>To view a screen shot of a student's PDF with properly formatted tables click <a href="misc/HTMLtoPDFInfo/SampleRes.gif" target="_blank">HERE. </a></li>

		</ul>



			<p><b>Titles, Headers, and Stylesheets</b></p>
		<ul>
		  	<li>YOU ARE NOT ALLOWED TO USE ANY STYLE, TITLE, SCRIPT, HTML, BODY, OR HEAD TAGS IN YOUR HTML. These will be automatically stripped!!</li>
			<li>H1 and H2 tags will be replaced with H3 and H4 tags, respectively.</li>
			<li>Employers have strongly requested to view applications in one package {printed (on paper) from the web} rather than 1 at a time, therefore PDF was the best choice to use. 
			Because some tags may cause conflict, whereas others cause errors, they will be stripped.</li>
			<li>It is in your best interest to ensure that your PDF version is properly formatted as this will be the only method of ensuring that everybody
			views your application properly and uniformly.</li>
		</ul>

		
			<p><b>Titles &amp; Table of Contents appear in PDF</b></p>
		<ul>
			<li>When applications are sent to employers, they are viewed as a whole (meaning all students). This is accompanied with the table of contents,
			the student's name, and either "Resume" or "Coverletter" on the top. These were left in when students were viewing their applications so they
			could get a feel of how the employers will see it as well. </li>
			<li>To make the application packages easily viewable by employers, a table of contents is included. This is automatically generated by the conversion program
			using H1 &amp; H2 tags as reference points. Your name uses H1, and then a title of either "Coverletter", "Resume", or "Transcript" uses H2, forming an indentation.</li>
			<li>To view a screen shot of what the employers see when viewing applications click <a href="misc/HTMLtoPDFInfo/PDFToC.gif" target="_blank">HERE.</a></li>
		</ul>

		</td>
	</tr>


</table>

