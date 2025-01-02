// PGM -- this shader is drawn in the background when the main menu is up
gfx/2d/mainmenuback
{
	{
		map gfx/2d/winback.tga
		rgbGen identity		
	}
}

gfx/2d/textends
{
	{
		map gfx/2d/textends.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//		rgbGen globalColor
		rgbGen	identity
		alphaGen globalAlpha
	}
}

// PGM -- large text dialog background
gfx/2d/talkmenu
{
	{
		map gfx/2d/talkmenu.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen	identity
		alphaGen globalAlpha
	}
}

// PGM -- small text dialog background
gfx/2d/talkmenu2
{
	{
		map gfx/2d/talkmenu2.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen	identity
		alphaGen globalAlpha
	}
}

// DHM -- Inventory shaders
gfx/2d/inventory_back
{
	cull none
	nopicmip
	surfaceparm nolightmap
	{
		//map gfx/2d/inventory4.tga
		map ui/card2_2.tga
	}
}

gfx/2d/inventory_hover
{
	cull none
	nopicmip
	surfaceparm nolightmap
	{
		//map gfx/2d/inventory5.tga
		map ui/card2_1.tga
		//blendfunc GL_ONE GL_ONE
		rgbGen	identity
	}
}

// pgm - this is the cat face on the level loading screen
// dhm - added nopicmip so it's not blurry when r_picmip > 0
gfx/2d/loading
{
	nopicmip
	surfaceparm nolightmap
	{
		map gfx/2d/loading.tga
		rgbGen identity
	}
}

ui/demondice
{
	nopicmip
	surfaceparm nolightmap
	{
		map ui/demondice.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

ui/health
{
	nopicmip
	surfaceparm nolightmap
	{
		clampmap ui/mana1_5.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

ui/healthback
{
	nopicmip
	surfaceparm nolightmap
	{
		clampmap ui/manabase.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

ui/healtharm
{
	nopicmip
	surfaceparm nolightmap
	cull none
	{
//		clampmap ui/needle.tga
		clampmap models/healtharm/skin01.tga
		blendfunc blend
		rgbGen identity
	}
}

ui/mana
{
	nopicmip
	surfaceparm nolightmap
	{
		clampmap ui/health1_1.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

ui/manaback
{
	nopicmip
	surfaceparm nolightmap
	{
		clampmap ui/healthbase.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

ui/manaarm
{
	nopicmip
	surfaceparm nolightmap
	cull none
	{
//		clampmap ui/needle.tga
		clampmap models/healtharm/skin02.tga
		blendfunc blend
		rgbGen identity
	}
}

gfx/mainmenu/alice1b_overlay
{
	nopicmip
	nomipmaps
	{
		nodepthtest
		clampmap gfx/mainmenu/alice1b.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen global
		alphagen global
	}
}

gfx/mainmenu/alice1c_overlay
{
	nopicmip
	nomipmaps
	{
		nodepthtest
		clampmap gfx/mainmenu/alice1c.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen global
		alphagen global
	}
}

gfx/mainmenu/alice2_overlay
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap gfx/mainmenu/alice2.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen global
		alphagen global
	}
}

gfx/mainmenu/alice3_overlay
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap gfx/mainmenu/alice3.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen global
		alphagen global
	}
}


gfx/mainmenu/alicebottom
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map gfx/mainmenu/alicebottom.tga
		rgbgen global
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

gfx/mainmenu/lightning
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map gfx/mainmenu/lightning.tga
		rgbGen wave noise 0 1 0 3
		blendfunc GL_ONE GL_ONE
	}
}

gfx/mainmenu/newgame
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map gfx/mainmenu/newgame.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

gfx/mainmenu/loadsave
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map gfx/mainmenu/loadsave.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

gfx/mainmenu/controls
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map gfx/mainmenu/controls.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

gfx/mainmenu/video
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map gfx/mainmenu/video.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

gfx/mainmenu/controls
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map gfx/mainmenu/controls.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

gfx/mainmenu/quit
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map gfx/mainmenu/quit.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

// SMF - hud shaders
ui/ui_main
{
	nopicmip
	nomipmaps
	{
		nodepthtest
//		depthwrite
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/pieces/main/health.tga
		rgbGen identity	
	}	
}

ui/ui_manamain
{
	nopicmip
	nomipmaps
	{
		nodepthtest
//		depthwrite
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/pieces/main/skin01.tga
		rgbGen identity	
	}
}

ui/ui_health
{	
	sort decal
	cull disable
	{
		nodepthtest
		map models/ui/pieces/main/skin_health02.tga
		rgbGen wave sin 1 .4 0 .1
		tcMod scroll -0.2 -0.2
	}
	{
		nodepthtest
		map textures/liquid/grey1.tga
		blendfunc gl_one gl_one
		tcmod scale .5 1.5
		tcmod scroll .3 .3
	}
	{
		nodepthtest
		map textures/liquid/grey2.tga
		blendfunc gl_one gl_one
		tcmod scale .5 1.5
		tcmod scroll -.3 -.3
	}	
}
ui/ui_mana
{	
	sort decal
	cull disable
	{
		nodepthtest
		map models/ui/pieces/main/skin_mana01.tga
		rgbGen wave sin 1 .3 0 .2
		tcMod scroll -0.16 -0.16
	}
	{
		nodepthtest
		map textures/liquid/grey1.tga
		blendfunc gl_one gl_one
		tcmod scale .5 1.5
		tcmod scroll .3 .3
	}
	{
		nodepthtest
		map textures/liquid/grey2.tga
		blendfunc gl_one gl_one
		tcmod scale .5 1.5
		tcmod scroll -.3 -.3
	}	
}

ui/ui_foldout
{
//	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/pieces/foldout/base.tga
	}
	//{
	//	map models/ui/pieces/foldout/glow.tga
	//	blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	//	alphagen wave sin .5 .5 0 .2
	//}	
}

ui/ui_back
{
	sort decal
	cull disable
	nopicmip
	nomipmaps
	{
		nodepthtest
		map models/ui/pieces/main/skin_back01.tga
	}
}


ui/loadgame/bigMask
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/load/bigMask.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/load/bigMask_glow.tga
		rgbgen identity
		alphaGen wave noise 0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/loadgame/lensCap
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/load/lensCap.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/loadgame/sepia1
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		animMap 10 ui/load/sepia1_2.tga ui/load/sepia1_3.tga ui/load/sepia1_4.tga ui/load/sepia1_5.tga ui/load/sepia1_6.tga
		rgbgen identity
		alphaGen wave noise 0.8 0.2 0 10
		blendfunc GL_DST_COLOR GL_SRC_ALPHA
	}
}

ui/loadgame/deleteconfirm
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/load/deleteconfirm.tga
		rgbgen identity
//		alphaGen wave sin 0 1 0.3 0.3
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcmod rotate 20 20			
	}
}

ui/control/glow1
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/glow_1.tga
		rgbgen identity
		alphaGen wave sin .6 .8 0 0.5
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/lightning
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map gfx/mainmenu/reflect_1.tga
		rgbGen wave noise 0 1 0 3
		blendfunc GL_ONE GL_ONE
	}
}

ui/control/reflect_1_lightning
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/reflect_1.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
//	surfaceparm nolightmap
//	{
//		nodepthtest
//		map ui/control/reflect_1_glow.tga
//		rgbgen identity
//		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
//		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//	}
}

ui/control/bar
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/bar_1.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/bar_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/bar_1_glow.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/backA
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/controlA.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/control_lightningA.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/backA
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
//		map ui/control/controlA.tga
		map ui/control/control_lightningA.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/control_lightningA.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/backB
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
//		map ui/control/controlB.tga
		map ui/control/control_lightningB.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/control_lightningB.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/backC
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
//		map ui/control/controlC.tga
		map ui/control/control_lightningC.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/control_lightningC.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/backD
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
//		map ui/control/controlD.tga
		map ui/control/control_lightningD.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/control_lightningD.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/backE
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
//		map ui/control/controlE.tga
		map ui/control/control_lightningE.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/control_lightningE.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/backF
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
//		map ui/control/controlF.tga
		map ui/control/control_lightningF.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/control_lightningF.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/reflect_lightning
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/reflect_1.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/reflect_1_glow.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/bulb_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/control/bulb_glow_1.tga
		rgbgen identity
//		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		alphaGen wave sin 0 1 0 0.5
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/glowing_eyes
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/eye_glow_1.tga
		rgbgen identity
//		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		alphaGen wave sin 0 1 0 0.2
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/glowing_heart
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/control/glow_1.tga
		rgbgen identity
		alphaGen wave sin 0 1 0.3 0.3
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/control/glowing_twirl
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/control/twirl_1.tga
		rgbgen identity
//		alphaGen wave sin 0 1 0.3 0.3
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcmod rotate 50 50			
	}
}

ui/control/glowing_twirl2
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/control/twirl_2.tga
		rgbgen identity
//		alphaGen wave sin 0 1 0.3 0.3
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcmod rotate 75 30
	}
}

ui/control/apply
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/apply_glow.tga
		alphaGen vertex
		rgbGen wave sin .7 .35 0 .2
		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_ONE
	}
}

ui/control/cancel
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/cancel_wavy.tga
		rgbgen identity
		alphaGen wave sin 0 1 0.8 0.3
//		alphaGen wave inversesawtooth -2 3 0 1
		blendfunc GL_SRC_ALPHA GL_ONE
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/cancel_wavy2.tga
		rgbgen identity
		alphaGen wave sin 0 1 0.4 0.2
//		alphaGen wave sawtooth 2 3 0 1
		blendfunc GL_SRC_ALPHA GL_ONE
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/cancel.tga
		rgbgen identity
		blendfunc GL_ONE GL_ONE
	}
}

ui/control/back
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/back_glow.tga
		alphaGen vertex
		rgbGen wave sin .7 .35 0.6 .2
		blendfunc GL_ONE_MINUS_SRC_ALPHA GL_ONE
	}
}

ui/control/skull_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/control/skull_eyes_glow.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE
	}
}

ui/main/loadgame_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/main/button_loadgame_glow.tga
		rgbgen identity
		rgbGen wave sin .7 .35 0 .2
		blendfunc GL_ONE GL_ONE
	}
}

ui/main/newgame
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/main/button_newgame.tga
		rgbgen identity
	}
}

ui/main/newgame_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/main/button_newgame_glow.tga
		rgbgen identity
		rgbGen wave sin .7 .35 0 .2
		blendfunc GL_ONE GL_ONE
	}
}

ui/main/settings_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/main/button_settings_glow.tga
		rgbgen identity
		rgbGen wave sin .7 .35 0 .2
		blendfunc GL_ONE GL_ONE
	}
}

ui/main/quit_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/main/button_quit_glow.tga
		rgbgen identity
		rgbGen wave sin .7 .35 0 .2
		blendfunc GL_ONE GL_ONE
	}
}

ui/main/mainA_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/mainA_glow.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/main/mainB_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/mainB_glow.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/main/mainD_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/mainD_glow.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/main/mainE_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/mainE_glow.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/main/wick
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/main/wick_glow.tga
		rgbgen identity
//		rgbGen wave noise 0 1 0 3 //0.8 0.2 0 10
		alphaGen wave sin .7 .35 0 .2
//		blendfunc GL_ONE GL_ONE
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/main/smoke
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/main/smoke_trail.tga
		rgbgen identity
//		rgbGen wave noise 0 1 0 3 //0.8 0.2 0 10
//		rgbGen wave sin .7 .35 0 .2
		tcMod scroll 0 0.1
		blendfunc GL_ONE GL_ONE
//		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/main/smokeMask
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/smoke_mask.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/smoke_mask_glow.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/deadtime_watch
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		map models/powerups/deadtime_watch/skin01.tga
		rgbgen lightingDiffuse
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}


ui/foldout/icewand
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/icewand.tga
	}
}
ui/foldout/cards
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/cards.tga
	}
}
ui/foldout/knife
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/knife.tga
	}
}
ui/foldout/mallet
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/mallet.tga
	}
}
ui/foldout/dice
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/dice1.tga
	}
}
ui/foldout/dice1
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/dice1.tga
	}
}
ui/foldout/dice2
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/dice2.tga
	}
}
ui/foldout/dice3
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/dice3.tga
	}
}
ui/foldout/jackbomb
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/jackbomb.tga
	}
}
ui/foldout/icewand
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/icewand.tga
	}
}
ui/foldout/jacks
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/jacks.tga
	}
}
ui/foldout/eyestaff
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/eyestaff.tga
	}
}
ui/foldout/blunderbuss
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/blunderbuss.tga
	}
}

ui/foldout/rage
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/ragebox.tga
	}
}
ui/foldout/tea
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/tea.tga
	}
}
ui/foldout/glass
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/glass.tga
	}
}
ui/foldout/watch
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		map models/ui/watch.tga
	}
}

ui/quit/credits_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/quit/credits_glow.tga
		rgbgen identity
		rgbGen wave sin .7 .35 0 .2
		blendfunc GL_ONE GL_ONE
	}
}
ui/quit/yes_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/quit/yes_glow.tga
		rgbgen identity
		rgbGen wave sin .7 .35 0 .2
		blendfunc GL_ONE GL_ONE
	}
}
ui/quit/no_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/quit/no_glow.tga
		rgbgen identity
		rgbGen wave sin .7 .35 0 .2
		blendfunc GL_ONE GL_ONE
	}
}

ui/back_button
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/back_button.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/back_button_glow.tga
		rgbgen identity
		rgbGen wave sin .7 .35 0 .2
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/back_button_hover
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/back_button_hover.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/back_button_glow.tga
		rgbgen identity
		rgbGen wave sin .7 .35 0 .2
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/newgame/easy_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/newgame/easy_glow.tga
		rgbgen identity
		alphaGen wave sin .7 .35 0 .2
		blendfunc GL_SRC_ALPHA GL_ONE
	}
}
ui/newgame/medium_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/newgame/medium_glow.tga
		rgbgen identity
		alphaGen wave sin .7 .35 0 .2
		blendfunc GL_SRC_ALPHA GL_ONE
	}
}
ui/newgame/hard_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/newgame/hard_glow.tga
		rgbgen identity
		alphaGen wave sin .7 .35 0 .2
		blendfunc GL_SRC_ALPHA GL_ONE
	}
}
ui/newgame/nightmare_glow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map ui/newgame/nightmare_glow.tga
		rgbgen identity
		alphaGen wave sin .7 .35 0 .2
		blendfunc GL_SRC_ALPHA GL_ONE
	}
}

textures/bind/leftarrow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map textures/bind/leftarrow.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
textures/bind/rightarrow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map textures/bind/rightarrow.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
textures/bind/uparrow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map textures/bind/uparrow.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
textures/bind/downarrow
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		map textures/bind/downarrow.tga
		rgbgen identity
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}



ui/main/glowA
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/glowA.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/main/glowB
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/glowB.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/main/glowC
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/glowC.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/main/glowD
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/glowD.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/main/glowE
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/glowE.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/main/glowF
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/main/glowF.tga
		rgbgen identity
		alphaGen wave noise 0 1 0 3 //0.8 0.2 0 10
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/pausewatch/watch
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		nodepthtest
		clampmap ui/pausewatch/pausewatch.tga
		rgbgen identity
//		alphaGen wave sin 0 1 0.3 0.3
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//		tcmod rotate 50 50			
	}
}

ui/credits/rabbit
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
//		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		blendfunc GL_ZERO GL_SRC_COLOR
		map ui/credits/rabbit.tga
	}
}
ui/credits/bill
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
//		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		blendfunc GL_ZERO GL_SRC_COLOR
		map ui/credits/bill.tga
	}
}

ui/control/game_options
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		map ui/control/game_options.tga
		alphaGen wave sin .7 .35 0 .2
//		blendfunc GL_SRC_ALPHA GL_ONE
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/control/video_options
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		map ui/control/video_options.tga
		alphaGen wave sin .7 .35 0.1 .2
//		blendfunc GL_SRC_ALPHA GL_ONE
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/control/audio_options
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		map ui/control/audio_options.tga
		alphaGen wave sin .7 .35 0.3 .2
//		blendfunc GL_SRC_ALPHA GL_ONE
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/control/control_options
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		nodepthtest
		map ui/control/control_options.tga
		alphaGen wave sin .7 .35 0.5 .2
//		blendfunc GL_SRC_ALPHA GL_ONE
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}


ui/credits/rogue_glow
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		clampmap ui/credits/rogue_logo.tga
		alphaGen wave sin .7 .35 0 .8
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/credits/ea_glow
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		clampmap ui/credits/ea_logo.tga
		alphaGen wave sin .7 .35 0.4 .8
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui/worldmap/star1
{
	sort decal
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	cull disable
	{
		nodepthtest
		clampmap ui/worldmap/star1.tga
		rgbGen wave noise 0 1 0 3
		blendFunc GL_ONE GL_ONE
	}
}

ui/loadgame/pushbtn_med_lit
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		clampmap ui/load/pushbtn_med_lit.tga
		alphaGen wave sin .9 0.1 0 0.5
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui/loadgame/pushbtn_large_lit
{
	sort decal
	nopicmip
	nomipmaps
	cull disable
	{
		clampmap ui/load/pushbtn_large_lit.tga
		alphaGen wave sin .9 0.1 0 1.5
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
