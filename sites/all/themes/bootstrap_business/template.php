<?php

/**
 * Page alter.
 */
function bootstrap_business_page_alter($page) {
    $mobileoptimized = array(
        '#type' => 'html_tag',
        '#tag' => 'meta',
        '#attributes' => array(
            'name' => 'MobileOptimized',
            'content' => 'width'
        )
    );
    $handheldfriendly = array(
        '#type' => 'html_tag',
        '#tag' => 'meta',
        '#attributes' => array(
            'name' => 'HandheldFriendly',
            'content' => 'true'
        )
    );
    $viewport = array(
        '#type' => 'html_tag',
        '#tag' => 'meta',
        '#attributes' => array(
            'name' => 'viewport',
            'content' => 'width=device-width, initial-scale=1'
        )
    );
    drupal_add_html_head($mobileoptimized, 'MobileOptimized');
    drupal_add_html_head($handheldfriendly, 'HandheldFriendly');
    drupal_add_html_head($viewport, 'viewport');
}

/**
 * Preprocess variables for html.tpl.php
 */
function bootstrap_business_preprocess_html(&$variables) {
    /**
     * Add IE8 Support
     */
    drupal_add_css(path_to_theme() . '/css/ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => '(lt IE 9)', '!IE' => FALSE), 'preprocess' => FALSE));

    drupal_add_css(path_to_theme() . '/css/bootstrap.min.css');
	drupal_add_css(path_to_theme() . '/css/print.css',array('media' => 'print'));
    /**
     * Add Javascript for enable/disable scrollTop action
     */
    if (theme_get_setting('scrolltop_display', 'bootstrap_business')) {

        drupal_add_js('jQuery(document).ready(function($) { 
		$(window).scroll(function() {
			if($(this).scrollTop() != 0) {
				$("#toTop").fadeIn();	
			} else {
				$("#toTop").fadeOut();
			}
		});
		
		$("#toTop").click(function() {
			$("body,html").animate({scrollTop:0},800);
		});	
		
		});', array('type' => 'inline', 'scope' => 'header'));
    }    
    if(arg(0) == 'employers' && arg(1)=="")
        $variables['classes_array'][] = 'companies';
    //EOF:Javascript
}

/**
 * Override or insert variables into the html template.
 */
function bootstrap_business_process_html(&$vars) {
    // Hook into color.module
    if (module_exists('color')) {
        _color_html_alter($vars);
    }
}

/**
 * Preprocess variables for page template.
 */
function bootstrap_business_preprocess_page(&$vars) {

    /**
     * insert variables into page template.
     */
    if ($vars['page']['sidebar_first'] && $vars['page']['sidebar_second']) {
        $vars['sidebar_grid_class'] = 'col-md-3';
        $vars['main_grid_class'] = 'col-md-6';
    } elseif ($vars['page']['sidebar_first'] || $vars['page']['sidebar_second']) {
        $vars['sidebar_grid_class'] = 'col-md-4';
        $vars['main_grid_class'] = 'col-md-8';
    } else {
        $vars['main_grid_class'] = 'col-md-12';
    }

    if ($vars['page']['header_top_left'] && $vars['page']['header_top_right']) {
        $vars['header_top_left_grid_class'] = 'col-md-8';
        $vars['header_top_right_grid_class'] = 'col-md-4';
    } elseif ($vars['page']['header_top_right'] || $vars['page']['header_top_left']) {
        $vars['header_top_left_grid_class'] = 'col-md-12';
        $vars['header_top_right_grid_class'] = 'col-md-12';
    }

    /**
     * Add Javascript
     */
    if ($vars['page']['pre_header_first'] || $vars['page']['pre_header_second'] || $vars['page']['pre_header_third']) {
        drupal_add_js('
	function hidePreHeader(){
            jQuery(".toggle-control").html("<a href=\"javascript:showPreHeader()\"><span class=\"glyphicon glyphicon-plus\"></span></a>");
            jQuery("#pre-header-inside").slideUp("fast");
	}
	function showPreHeader() {
            jQuery(".toggle-control").html("<a href=\"javascript:hidePreHeader()\"><span class=\"glyphicon glyphicon-minus\"></span></a>");
            jQuery("#pre-header-inside").slideDown("fast");
	}
	', array('type' => 'inline', 'scope' => 'footer', 'weight' => 3));
    }

    drupal_add_js('
        jQuery(document).ready(function() { 
            $("#show-block").click(function () {
                if ($("#user-info").is(":visible")) {
                    $(this).html($(this).html().replace(/Hide/, "Show"));
                    $(this).addClass("showarrow");
                    $(this).removeClass("hidearrow");
                } else {
                    $(this).html($(this).html().replace(/Show/, "Hide"));
                    $(this).addClass("hidearrow");
                    $(this).removeClass("showarrow");
                }
                // Do it afterwards as the operation is async
                $("#user-info").slideToggle("fast");
            });
        });
    ', array('type' => 'inline', 'scope' => 'header'));
     //EOF:Javascript

    global $user;
    global $base_url;

    if (in_array('student', $user->roles))
        $path = $base_url. '/student/account';
    else if (in_array('employer', $user->roles))
        $path = $base_url. '/employer/account';
    else if (in_array('faculty', $user->roles)) 
        $path = $base_url. '/faculty/account';
    else if (in_array('faculty_admin', $user->roles)) 
        $path = $base_url. '/admin/config/people/faculty/account';

    if ($user->uid != 1 && !in_array('faculty_admin', $user->roles)) { 
        if ($user->uid != 0) {
            // code for the account and logout button
            $vars['primary_links'] = '
                    <div class="hello-msg"><span>Hello </span>' . $user->name . '</div>
                    <a href="#" id="show-block">&nbsp;</a>
                    <ul id="user-info" style="display:none">
                        <li><a href="' . $path . '">My Account</a></li>
                        <li><a href="' . $base_url . '/user/logout">Logout</a></li>
                    </ul>';
        } else {
            // code for the login button
            $vars['primary_links'] = '<ul>
                        <li><a href="' . $base_url . '/user">Login</a></li>
                        <li>Employer ? <a href="' . $base_url . '/employer/register">Sign Up</a></li>
                    </ul>';
        }
    }
            
    $main_menu = menu_navigation_links('main-menu');
    $vars['theme_menu'] = theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('class' => array('main-menu', 'menu')), 'heading' => array('text' => t('Main menu'), 'level' => 'h2', 'class' => array('element-invisible'))));
    
}

/**
 * Override or insert variables into the page template.
 */
function bootstrap_business_process_page(&$variables) {
    // Hook into color.module.
    if (module_exists('color')) {
        _color_page_alter($variables);
    }
}

/**
 * Preprocess variables for block.tpl.php
 */
function bootstrap_business_preprocess_block(&$variables) {
    $variables['classes_array'][] = 'clearfix';
}

/**
 * Override theme_breadrumb().
 *
 * Print breadcrumbs as a list, with separators.
 */
function bootstrap_business_breadcrumb($variables) {
    $breadcrumb = $variables['breadcrumb'];
    if (!empty($breadcrumb)) {
        $breadcrumb[] = drupal_get_title();
        $breadcrumbs = '<ol class="breadcrumb">';
        $count = count($breadcrumb) - 1;
        foreach ($breadcrumb as $key => $value) {
            $breadcrumbs .= '<li>' . $value . '</li>';
        }
        $breadcrumbs .= '</ol>';
        return $breadcrumbs;
    }
}

/**
 * Search block form alter.
 */
function bootstrap_business_form_alter(&$form, &$form_state, $form_id) {
    if ($form_id == 'search_block_form') {
        unset($form['search_block_form']['#title']);
        $form['search_block_form']['#title_display'] = 'invisible';
        $form_default = t('Search this website...');
        $form['search_block_form']['#default_value'] = $form_default;
        $form['actions']['submit']['#attributes']['value'][] = '';
        $form['search_block_form']['#attributes'] = array('onblur' => "if (this.value == '') {this.value = '{$form_default}';}", 'onfocus' => "if (this.value == '{$form_default}') {this.value = '';}");
    }
}

/* Custom menu link modifications */
function bootstrap_business_menu_link(array $variables) { 
    $element = $variables['element'];    
    $sub_menu = '';
    $parent = $element['#original_link']['has_children'];

    if ($element['#below']) {
        $sub_menu = drupal_render($element['#below']);
    }
    
    if( $parent == 1 ){      
        $output = '<h3>'.$element['#title'].'</h3>';
    }else
        $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    
    return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/* limit slideshow images */
function node_load_by_type($type, $limit, $offset = 0) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', $type)
            ->range($offset, $limit);
    $results = $query->execute();
    return node_load_multiple(array_keys($results['node']));
}