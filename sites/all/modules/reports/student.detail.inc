<?php
/**
 * Student Summary Report Function.
 */
function ajax_qualification_detail_callback($form, $form_state) {
    return $form['filter']['wrapper']['highest']['qualification_level'];
}
function detail_report_select_ajax($form, $form_state) {
    return $form['filter']['wrapper'];
}
function detail_reports_submit($form, &$form_state) {  
   $form_state['rebuild'] = TRUE;
}
function detail_reports($form, &$form_state) { 
     global $base_url;
     $display="";
     drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
     drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
     drupal_add_js('jQuery(document).ready(function($) { 
        $("#from").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
          $("#to").datepicker("option","minDate", selected)
        }
        });
        $("#to").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
         $("#from").datepicker("option","maxDate", selected)
        }
        }); 
        $(".popup").click(function () {                    
          window.open($(this).attr("href"), "PopupWindow", "width=600,height=600,scrollbars=yes,resizable=no");
          return false;
        });
        $("#download").click(function() {
         $("#download_pdf").val("pdf");
         document.getElementById("detail-reports").submit();
        });
        });', 'inline');
     drupal_add_js('function submit_change(){document.getElementById("download_pdf").value=""}','inline');
    $form = array();
    $form['filter']['title_links'] = array(
      '#type' => 'fieldset',
      '#title' => t('<ul class="student-detail-tabs">
              <li><a class="active" href="studentregisteredreports">No.of students registered in the portal</a></li>
              <li><a href="studentappliedreports">No.of students applied for jobs</a></li>
              <li><a href="studentinterviewreports">No.of students shortlisted for interview</a></li>
              <li><a href="studentemployedreports">No.of students employed</a></li>
              <li><a href="studenpsychometrictestreports">No.of students took psychometric test</a></li>
              </ul>'),
    );
    $form['filter']['wrapper'] = array(
     '#prefix' => '<div  class="student-detail-report">',
     '#suffix' => '</div>',
    );
    $form['filter']['download_pdf'] = array(
      '#type' => 'hidden',
      '#attributes' => array(
        'id' => 'download_pdf'
      )
    );
    $form['filter']['wrapper']['daterange'] = array(
      '#type' => 'fieldset',
      '#title' => t('Date Range'),
    );
    $form['filter']['wrapper']['daterange']['from'] = array(
    '#type' => 'textfield', 
    '#id' => 'from', 
    '#title' => 'From',
    '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['wrapper']['daterange']['to'] = array(
    '#type' => 'textfield', 
    '#id' => 'to', 
    '#title' => 'To',
    '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['wrapper']['matric_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Matric No'),
    );
    $form['filter']['wrapper']['student_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    );
    $form['filter']['wrapper']['gender'] = array(
    '#type' => 'radios',
    '#title' => t('Gender'),
    '#options' => array('M'=>'Male','F'=>'Female'),
    );
    $form['filter']['wrapper']['year_of_study_from'] = array(
    '#type' => 'select',
    '#title' => t('Year of Study'),
    '#options' => all_years(),
    );
   $form['filter']['wrapper']['year_of_study_to'] = array(
    '#type' => 'select',
    '#options' => all_years(),
    );    
   $form['filter']['wrapper']['highest']['qualification'] = array(
    '#type' => 'select',
    '#id' => 'qualification',
    '#title' => 'Programme',
    '#options' =>  array(''=>'Select') + qualification_list(),   
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'ajax_qualification_detail_callback',
      'wrapper' => 'dropdown-highest-degree',
    ),  
  );
    $selected = isset($form_state['values']['qualification']) ? $form_state['values']['qualification'] : $qualification[0];
    $form['filter']['wrapper']['highest']['qualification_level'] = array(
    '#type' => 'select',
    '#id' => 'qualificationlevel',
    '#prefix' => '<div id="dropdown-highest-degree">',
    '#suffix' => '</div>',
    '#options' => array(''=>'Select') +highest_qualification_list($selected),
    );
    $form['filter']['wrapper']['Kulliyyah'] = array(
    '#type' => 'textfield',
    '#title' => 'Kulliyyah',
    ); 
    $form['filter']['wrapper']['status'] = array(
    '#type' => 'select',
    '#options' => array(''=>'select status',1=>'Active',0=>'Inactive'),
    '#title' => 'Status',
    );
    $form['filter']['wrapper']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('detail_reports_submit'),
      '#attributes' => array('onclick' => 'javascript:submit_change();'),
    );
    $form['filter']['print_report'] = array(
      '#type' => 'fieldset',
      '#title' => t('<span><a href="javascript:window.print();">Print Report</a>&nbsp;|&nbsp;<a id="download">Download PDF</a></span>'), 
    );
     $header = array(
             array('data' => t('Student ID'), 'width' => '10%'), 
             array('data' => t('Matric no'), 'width' => '5%'),
             array('data' => t('Name'), 'width' => '20%'),
             array('data' => t('Gender'), 'width' => '5%'),
             array('data' => t('Year of Study '), 'width' => '15%'),
             array('data' => t('Programme'), 'width' => '30%'),
             array('data' => t('Kulliyyah'), 'width' => '5%'),
             array('data' => t('Options'), 'width' => '10%'),      
           );
    
      $results = db_select('stdinfo','s');
      $results->leftJoin('student_details', 'd', 'd.uid = s.uid');
      $results->leftJoin('student_qualification', 'q', 's.uid = q.uid and q.highest=1');
      $results->leftJoin('stdprogram', 'p', 's.id = p.stdid');
      $results->leftJoin('users', 'u', 's.uid = u.uid');
      $results->fields('s',array('uid','matric_no','name','gender')); 
      $results->fields('p',array('program_desc')); 
      $results->fields('q',array('graduation_date','qualification','qualification_level')); 
      
       if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
        {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0]." ".date("H:i:s");
          $results->condition('d.created_date',$fromdate,'>=');
        }
       if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
        {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0]." ".date("H:i:s");   
          $results->condition('d.created_date',$todate,'<=');
        }
       if(isset($form_state['values']['matric_number'])&& $form_state['values']['matric_number']!="")
        {
          $results->condition('s.matric_no','%'.$form_state['values']['matric_number'].'%','LIKE');
        }  
       if(isset($form_state['values']['student_name'])&& $form_state['values']['student_name']!="")
        {
          $results->condition('s.name','%'.$form_state['values']['student_name'].'%','LIKE');
        }
       if(isset($form_state['values']['gender'])&& $form_state['values']['gender']!="")
        {
          $results->condition('s.gender',$form_state['values']['gender'],'LIKE');
        }  
       if(isset($form_state['values']['year_of_study_from'])&& $form_state['values']['year_of_study_from']!="")
        {
          $results->condition('q.graduation_date',$form_state['values']['year_of_study_from'].'-01'.'-01','<=');
        }   
       if(isset($form_state['values']['year_of_study_to'])&& $form_state['values']['year_of_study_to']!="")
        {
          $results->condition('q.graduation_date',$form_state['values']['year_of_study_to'].'-01'.'-01','>=');
        }
       if(isset($form_state['values']['qualification'])&& $form_state['values']['qualification']!="")
        {
          $results->condition('q.qualification',$form_state['values']['qualification'],'=');
        }  
       if(isset($form_state['values']['qualification_level'])&& $form_state['values']['qualification_level']!="")
        {
          $results->condition('q.qualification_level',$form_state['values']['qualification_level'],'=');
        }  
       if(isset($form_state['values']['Kulliyyah'])&& $form_state['values']['Kulliyyah']!="")
        {
          $results->condition('p.program_desc','%'.$form_state['values']['Kulliyyah'].'%','LIKE');
        } 
       if(isset($form_state['values']['status'])&& $form_state['values']['status']!="")
        {
          $results->condition('u.status',$form_state['values']['status'],'=');
        } 
      $table_sort = $results->extend('TableSort') // Add table sort extender.
                      ->orderByHeader($header); // Add order by headers.

      $pager = $table_sort->extend('PagerDefault')
                       ->orderBy('s.id', 'DESC');
       if(isset($form_state['values']['download_pdf']) && $form_state['values']['download_pdf']=='pdf')
         $pager->limit(200000000); 
       else
         $pager->limit(10); //10 rows per page.

      $result = $pager->execute();
      $qualification_list=qualification_list();
      foreach($result as $res):
        if($res->graduation_date!="")
         $graduation_date=date("d-m-Y",strtotime($res->graduation_date));
        else
         $graduation_date='';   
        $highest_qualification_list=highest_qualification_list($res->qualification);    
        $rows[] =array($res->uid,$res->matric_no,$res->name,$res->gender,$graduation_date,$qualification_list[$res->qualification]."<br>".$highest_qualification_list[$res->qualification_level],$res->program_desc,'<a href="'.$base_url.'/detail/latestresume/'.encrypt($res->uid,variable_get('IIUM_Encryption_Key', NULL)).'" class="popup" alt="VIEW" title="VIEW">View CV</a>');
        $pdf_records.="<tr><td>".$res->uid."</td><td>".$res->matric_no."</td><td>".$res->name."</td><td>".$res->gender."</td><td>".$graduation_date."</td><td>".$qualification_list[$res->qualification]."<br>".$highest_qualification_list[$res->qualification_level]."</td><td>".$res->program_desc."</td></tr>";
      endforeach;  
      $form['filter']['wrapper_table'] = array(
     '#prefix' => '<div class="wrapper_table">',
     '#suffix' => '</div>',
     );  
     $form['filter']['wrapper_table']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No Records Found!')
    );
    
    $form['filter']['wrapper_table']['pager'] = array('#markup' => theme('pager')); 
    $_SESSION['pdf_records']=$pdf_records;
   if(isset($form_state['values']['download_pdf']) && $form_state['values']['download_pdf']=='pdf')
    {
         $markup = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
        <html xmlns="http://www.w3.org/1999/xhtml">
        <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<style>
table.student-detail{border-collapse: collapse; border-color: #ccc; border-width: 1px; color: #333333; font-family: verdana,arial,sans-serif; font-size: 11px;}
table.student-detail tr{background-color:#e8e8e8;}
table.student-detail th{background-color:#dddddc; border-color: #ccc; border-style: solid; border-width: 1px; padding: 8px;}
table.student-detail td{border-color: #ccc; border-style: solid; border-width: 1px; padding: 8px; background:#f7f7f5; font-size:11px;}
</style>
        </head>
        <body>
        <div style="text-align:center">
		<table width="95%" border="0" cellspacing="0" cellpadding="0" class="student-detail">
		<tr>
		<th width="10%">Student ID</th>
		<th width="5%">Matric no</th>
		<th width="20%">Name</th>
		<th width="5%">Gender</th>
		<th width="15%">Year of Study</th>
        <th width="30%">Programme</th>
		<th width="10%">Kulliyyah</th></tr>'.$_SESSION['pdf_records'].'</table></div>
        </body>
        </html>';
        require_once("sites/all/modules/print/lib/dompdf/dompdf_config.inc.php");
        $dompdf = new DOMPDF;
        $dompdf->load_html($markup);
        $dompdf->render();
        $dompdf->stream("Student_Detail_Report.pdf");
    }
    return $form;
}

function all_years()
{
    $year_array['']='select year';  
   for($i=1991;$i<=2015;$i++)
   {
     $year_array[$i]=$i;  
   }
   return $year_array;
}
function all_employers()
{
  $employer_registration = db_select('employer_registration')
        ->fields('employer_registration',array('uid','company_organization')) 
        ->execute()->fetchAll();     
  $employer_registration_arr[''] = 'Select';
        foreach ($employer_registration as $value):
            $employer_registration_arr[$value->uid] =$value->company_organization;
        endforeach;
  return $employer_registration_arr;  
}
function psychometric_sections()
{
  $psychometric_sections = db_select('psychometric_sections')
        ->fields('psychometric_sections',array('id','title')) 
        ->condition('id',1,'!=')->execute()->fetchAll();     
  $psychometric_sections_arr[''] = 'Select';
        foreach ($psychometric_sections as $value):
            $psychometric_sections_arr[$value->id] =$value->title;
        endforeach;
  return $psychometric_sections_arr;
}