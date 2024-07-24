package dlsym

import "C"

type DlSym struct {
	h       uintptr
	symList map[string]uintptr // unsafe.Pointer
}

func Open(name string) (*DlSym, error) {
	c, err := openSym(name)
	if err != nil {
		return nil, err
	} else {
		// log.Printf("library handle %X", c.h)
	}

	return c, nil
}

func newDlSym(h uintptr) *DlSym {
	return &DlSym{
		h:       h,
		symList: map[string]uintptr{},
	}
}

func (c *DlSym) Close() {
	closeSym(c)
}

func (c *DlSym) Lookup(name string) (uintptr, error) {
	if v, ok := c.symList[name]; ok {
		return v, nil
	}

	v, err := lookupSym(c, name)
	if err != nil {
		return uintptr(0), err
	}

	c.symList[name] = v
	return v, nil
}
