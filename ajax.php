<?php
ob_start();
$action = $_GET['action'];
include 'admin_class.php';
$crud = new Action();

if($action == 'login'){
	$login = $crud->login();
	if($login)
		echo $login;
}
if($action == 'login2'){
	$login = $crud->login2();
	if($login)
		echo $login;
}
if($action == 'logout'){
	$logout = $crud->logout();
	if($logout)
		echo $logout;
}
if($action == 'logout2'){
	$logout = $crud->logout2();
	if($logout)
		echo $logout;
}
if($action == 'save_user'){
	$save = $crud->save_user();
	if($save)
		echo $save;
}
if($action == 'delete_user'){
	$save = $crud->delete_user();
	if($save)
		echo $save;
}
if($action == 'signup'){
	$save = $crud->signup();
	if($save)
		echo $save;
}
if($action == "save_settings"){
	$save = $crud->save_settings();
	if($save)
		echo $save;
}
if($action == "save_quiz"){
	$save = $crud->save_quiz();
	if($save)
		echo $save;
}
if($action == "delete_quiz"){
	$save = $crud->delete_quiz();
	if($save)
		echo $save;
}
if($action == "save_question"){
	$save = $crud->save_question();
	if($save)
		echo $save;
}
if($action == "delete_question"){
	$save = $crud->delete_question();
	if($save)
		echo $save;
}
if($action == "check_quiz_exist"){
	$chk = $crud->check_quiz_exist();
	if($chk)
		echo $chk;
}
if($action == "save_participant"){
	$save = $crud->save_participant();
	if($save)
		echo $save;
}
if($action == "disconnect_all"){
	$save = $crud->disconnect_all();
	if($save)
		echo $save;
}
if($action == "start_quiz"){
	$save = $crud->start_quiz();
	if($save)
		echo $save;
}
if($action == "start_quiz_next"){
	$save = $crud->start_quiz_next();
	if($save)
		echo $save;
}
if($action == "finish_quiz"){
	$save = $crud->finish_quiz();
	if($save)
		echo $save;
}
if($action == "save_answer"){
	$save = $crud->save_answer();
	if($save)
		echo $save;
}
if($action == "load_answered"){
	$list = $crud->load_answered();
	if($list)
		echo $list;
}
if($action == "load_my_answer"){
	$list = $crud->load_my_answer();
	if($list)
		echo $list;
}