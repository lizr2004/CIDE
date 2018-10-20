#include <gtk/gtk.h>
#include <Scintilla.h>
#include <SciLexer.h>
#define PLAT_GTK 1
#include <ScintillaWidget.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#define MARGIN_FOLD_INDEX 1
#define SSM(m, w, l) scintilla_send_message(sci, m, w, l)
void gdk_trans_pixbuf(GdkPixbuf *p1, GdkPixbuf * p2, guint32 px){
guchar *px1,*px2;
g_return_if_fail (GDK_IS_PIXBUF(p1)&&GDK_IS_PIXBUF(p2));
px1=gdk_pixbuf_get_pixels(p1);
px2=gdk_pixbuf_get_pixels(p2);
int h = gdk_pixbuf_get_height(p2);
int w = gdk_pixbuf_get_width(p2);
while(h--){
for(int i=0;i<w;i++,px1+=3,px2+=3){
	if(px2[0]==0xff && px2[1]==0xff && px2[2]==0xff){
		for(int j=0;j<3;j++)		
		px2[j]=px1[j];
	}
}
px1 += gdk_pixbuf_get_rowstride (p1);
px2 += gdk_pixbuf_get_rowstride (p2);
}
}

int ct;
GdkPixbuf* imgbuf;
void handler(GtkWidget * a, gint b, SCNotification *notify, gpointer userData) {
    ScintillaObject *sci = SCINTILLA(a);
if(ct++>15){
    printf("handle\n");
    int x, y, w, h;
    gdk_window_get_origin (a->window, &x, &y); //get origin point
    gdk_drawable_get_size (GDK_DRAWABLE (a->window), &w, &h);
    GdkPixbuf *pbuf = gdk_pixbuf_get_from_drawable(0, a->window, 0, 0, 0, 0, 0, w, h);
    char buf[256];
    sprintf(buf, "/tmp/a%d.jpg", ct++);
    gdk_pixbuf_save (pbuf, buf, "jpeg", NULL, "quality", "100", NULL);
    //gdk_pixbuf_transparent(pbuf,0xFFFFFF00);
    //gdk_trans_pixbuf(imgbuf, pbuf, 0xFFFFFF);
    gdk_draw_pixbuf(a->window, 0, imgbuf, 0, 0, 0, 0, w, h, GDK_RGB_DITHER_NORMAL, 0, 0);
    g_object_unref(pbuf);
}
    const int modifiers = notify->modifiers;
    const int position = notify->position;
    const int margin = notify->margin;
    const int line_number = SSM(SCI_LINEFROMPOSITION, position, 0);
    switch (margin) {
    case 1:
    {
        SSM(SCI_TOGGLEFOLD, line_number, 0);
    }
    break;
    }
}
static int exit_app(GtkWidget*w, GdkEventAny*e, gpointer p) {
    gtk_main_quit();
    return w || e || p || 1;	// Avoid warnings
}
int ev(GtkWidget*a, GdkEventAny*e, gpointer p){
return 0;
}
int main(int argc, char **argv) {
    GtkWidget *app;
    GtkWidget *editor;
    ScintillaObject *sci;
    gtk_init(&argc, &argv);
    imgbuf = gdk_pixbuf_new_from_file("a.jpg", 0);
    app = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    editor = scintilla_new();
    sci = SCINTILLA(editor);
    gtk_container_add(GTK_CONTAINER(app), editor);
    gtk_signal_connect(GTK_OBJECT(app), "delete_event",
                       GTK_SIGNAL_FUNC(exit_app), 0);
    scintilla_set_id(sci, 0);
    gtk_widget_set_usize(editor, 500, 300);
    gtk_widget_set_colormap(editor, gdk_screen_get_rgba_colormap(gtk_widget_get_screen(app)));
    //g_signal_connect(G_OBJECT(editor), "sci-notify", G_CALLBACK(handler), NULL);
    //g_signal_connect(G_OBJECT(editor), "expose-event", G_CALLBACK(ev), NULL);
    SSM(SCI_STYLECLEARALL, 0, 0);
     for(int i=0;i<1024;i++)
     SSM(SCI_STYLESETBACK,i,1);
    SSM(SCI_SETLEXER, SCLEX_CPP, 0);
    SSM(SCI_SETKEYWORDS, 0, (sptr_t)"int char");
    SSM(SCI_STYLESETFORE, SCE_C_COMMENT, 0x008000);
    SSM(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x008000);
    SSM(SCI_STYLESETFORE, SCE_C_NUMBER, 0x808000);
    SSM(SCI_STYLESETFORE, SCE_C_WORD, 0x800000);
    SSM(SCI_STYLESETFORE, SCE_C_STRING, 0x800080);
    SSM(SCI_STYLESETBOLD, SCE_C_OPERATOR, 1);
    SSM(SCI_INSERTTEXT, 0, (sptr_t)
        "int main(int argc, char **argv) {\n"
        "    // Start up the gnome\n"
        "    gnome_init(\"stest\", \"1.0\", argc, argv);\n}"
       );
    SSM(SCI_SETPROPERTY, (sptr_t)"fold", (sptr_t)"1");
    SSM(SCI_SETPROPERTY, (sptr_t)"fold.compact", (sptr_t)"0");
    SSM(SCI_SETPROPERTY, (sptr_t)"fold.comment", (sptr_t)"1");
    SSM(SCI_SETMARGINTYPEN, MARGIN_FOLD_INDEX, SC_MARGIN_SYMBOL);//页边类型
    SSM(SCI_SETMARGINMASKN, MARGIN_FOLD_INDEX, SC_MASK_FOLDERS); //页边掩码
    SSM(SCI_SETMARGINWIDTHN, MARGIN_FOLD_INDEX, 15); //页边宽度
    // 折叠标签样式
    SSM(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PLUS);
    SSM(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
    SSM(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND,  SC_MARK_BOXPLUSCONNECTED);
    SSM(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED);
    SSM(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE);
    SSM(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
    SSM(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE);
    // 折叠标签颜色
    SSM(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, 0xa0a0a0);
    SSM(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, 0xa0a0a0);
    SSM(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, 0xa0a0a0);
    SSM(SCI_SETFOLDFLAGS, 16 | 4, 0); //如果折叠就在折叠行的上下各画一条横线
    SSM(SCI_SETMARGINSENSITIVEN, MARGIN_FOLD_INDEX, 1); //响应鼠标消息
    gtk_widget_show_all(app);
    gtk_widget_grab_focus(GTK_WIDGET(editor));
    gtk_main();
    return 0;
}
